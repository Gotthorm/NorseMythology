// SBMOBJECT.CPP

#include "Valhalla.h"
#include <memory.h>
#include <fbxsdk.h>
#include "Platform.h"

using namespace fbxsdk;

// Link the static non-dll version of the FBX library
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")

FbxNode* FindMeshNode( FbxNode* pNode )
{
	if( pNode->GetNodeAttribute() )
	{
		if( pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh )
		{
			return pNode;
		}
	}

	const int lChildCount = pNode->GetChildCount();
	for( int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex )
	{
		FbxNode* meshNode = FindMeshNode( pNode->GetChild( lChildIndex ) );

		if( meshNode )
		{
			return meshNode;
		}
	}

	return nullptr;
}

namespace Valhalla
{
	FBXObject::~FBXObject()
	{
		delete[] m_RenderData;
	}

	bool FBXObject::Load( const std::string& fileName )
	{
		bool results = false;

		FbxManager* pManager = FbxManager::Create();

		if( pManager )
		{
			FBXSDK_printf( "Autodesk FBX SDK version %s\n", pManager->GetVersion() );

			FbxImporter* importer = FbxImporter::Create( pManager, "" );

			if( importer )
			{
				if( importer->Initialize( fileName.c_str() ) == false )
				{
					FBXSDK_printf( "Error: Unable to load FBX file!\n" );
				}
				else
				{
					FbxScene* pScene = FbxScene::Create( pManager, "Valhalla Import Scene" );

					if( pScene )
					{
						if( importer->Import( pScene ) == true )
						{
							// Convert Axis System to what is used in this example, if needed
							FbxAxisSystem SceneAxisSystem = pScene->GetGlobalSettings().GetAxisSystem();
							FbxAxisSystem OurAxisSystem( FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded );
							if( SceneAxisSystem != OurAxisSystem )
							{
								OurAxisSystem.ConvertScene( pScene );
							}

							// Convert Unit System to what is used in this example, if needed
							FbxSystemUnit SceneSystemUnit = pScene->GetGlobalSettings().GetSystemUnit();
							if( SceneSystemUnit.GetScaleFactor() != 1.0 )
							{
								//The unit in this example is centimeter.
								FbxSystemUnit::cm.ConvertScene( pScene );
							}

							// Convert mesh, NURBS and patch into triangle mesh
							FbxGeometryConverter lGeomConverter( pManager );
							lGeomConverter.Triangulate( pScene, /*replace*/true );

							FbxNode* pMeshNode = FindMeshNode( pScene->GetRootNode() );

							if( pMeshNode )
							{
								FbxMesh* mesh = pMeshNode->GetMesh();

								if( mesh )
								{
									// We already triangulated the data, so poly count is actually the triangle count
									const int triangleCount = mesh->GetPolygonCount();

									// Each triangle has 3 vertices
									const int vertexCount = triangleCount * 3;
									int normalCount = 0;

									FbxGeometryElementNormal* geometryElementNormal = nullptr;
									FbxLayerElement::EMappingMode mappingMode = FbxLayerElement::eNone;
									FbxLayerElement::EReferenceMode referenceMode;

									if( mesh->GetElementNormalCount() > 0 )
									{
										geometryElementNormal = mesh->GetElementNormal( 0 );
										PLATFORM_ASSERT( geometryElementNormal );

										mappingMode = geometryElementNormal->GetMappingMode();

										// The normal mapping modes:
										//eNone
										//eByControlPoint
										//eByPolygonVertex
										//eByPolygon
										//eByEdge
										//eAllSame

										// For now we will only support eByControlPoint and eByPolygonVertex
										switch( mappingMode )
										{
										case FbxLayerElement::eByControlPoint:
										case FbxLayerElement::eByPolygonVertex:
											// Basically equates to one normal per vertex
											normalCount = vertexCount;
											break;
										default:
											PLATFORM_ASSERT( !"Unsupported mapping mode for normal data" );
											break;
										}
										
										// The normal reference modes
										//eDirect
										//eIndex
										//eIndexToDirect
										referenceMode = geometryElementNormal->GetReferenceMode();
										PLATFORM_ASSERT( referenceMode != FbxLayerElement::eIndex );
									}

									// Each vertex has 4 floats
									// Each normal has 3 floats
									const int dataSize = (vertexCount * 4 + normalCount * 3) * sizeof( float );
									m_RenderData = new unsigned char[ dataSize ];

									// Float data begins first
									float* positionVertex = reinterpret_cast<float*>(m_RenderData);

									// Normal data is appended to the end of the float data
									float* normalVertex = &( positionVertex[ vertexCount * 4 ] );

									int vertexCounter = 0;
									int polygonVertexIndex;
									int normalIndex;
									// Copy the vertex and normal data one triangle at a time
									for( int triangleIndex = 0; triangleIndex < triangleCount; ++triangleIndex )
									{
										for( int verticeIndex = 0; verticeIndex < 3; ++verticeIndex )
										{
											polygonVertexIndex = mesh->GetPolygonVertex( triangleIndex, verticeIndex );

											*( positionVertex++ ) = mesh->GetControlPointAt( polygonVertexIndex ).mData[ 0 ];
											*( positionVertex++ ) = mesh->GetControlPointAt( polygonVertexIndex ).mData[ 1 ];
											*( positionVertex++ ) = mesh->GetControlPointAt( polygonVertexIndex ).mData[ 2 ];
											
											// Ensure we are building homogeneous coordinates
											*( positionVertex++ ) = 1.0f;

											if( geometryElementNormal && normalCount > 0 )
											{
												if( mappingMode == FbxLayerElement::eByControlPoint )
												{
													if( referenceMode == FbxLayerElement::eDirect )
													{
														normalIndex = polygonVertexIndex;
													}
													else if( referenceMode == FbxLayerElement::eIndexToDirect )
													{
														normalIndex = geometryElementNormal->GetIndexArray().GetAt( polygonVertexIndex );
													}
												}
												else if( mappingMode == FbxLayerElement::eByPolygonVertex )
												{
													if( referenceMode == FbxLayerElement::eDirect )
													{
														normalIndex = vertexCounter++;
													}
													else if( referenceMode == FbxLayerElement::eIndexToDirect )
													{
														normalIndex = geometryElementNormal->GetIndexArray().GetAt( vertexCounter++ );
													}
												}

												*( normalVertex++ ) = geometryElementNormal->GetDirectArray().GetAt( normalIndex ).mData[ 0 ];
												*( normalVertex++ ) = geometryElementNormal->GetDirectArray().GetAt( normalIndex ).mData[ 1 ];
												*( normalVertex++ ) = geometryElementNormal->GetDirectArray().GetAt( normalIndex ).mData[ 2 ];
											}
										}
									}

									m_Size = dataSize;
									m_Vertices = vertexCount;

									results = true;
								}
							}
						}
					}
				}
			}
		}

		return results;
	}
}