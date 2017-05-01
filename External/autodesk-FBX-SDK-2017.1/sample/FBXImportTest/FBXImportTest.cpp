// FBXImportTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fbxsdk.h>
#include <assert.h>

using namespace fbxsdk;

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

int main()
{
	FbxManager* pManager = FbxManager::Create();
	
	if( !pManager )
	{
		FBXSDK_printf( "Error: Unable to create FBX Manager!\n" );
		exit( 1 );
	}

	FBXSDK_printf( "Autodesk FBX SDK version %s\n", pManager->GetVersion() );

	FbxScene* pScene = FbxScene::Create( pManager, "My Scene" );

	if( !pScene )
	{
		FBXSDK_printf( "Error: Unable to create FBX scene!\n" );
		exit( 1 );
	}

	FbxImporter* mImporter = FbxImporter::Create( pManager, "" );

	if( !mImporter )
	{
		FBXSDK_printf( "Error: Unable to create FBX importer!\n" );
		exit( 1 );
	}

	if( mImporter->Initialize( "humanoid.fbx" ) == false )
	{
		FBXSDK_printf( "Error: Unable to load FBX file!\n" );
		exit( 1 );
	}

	if( mImporter->Import( pScene ) == true )
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

		FbxNode* root = pScene->GetRootNode();

		FbxNode* pMeshNode = FindMeshNode( root );

		if( pMeshNode )
		{
			FbxMesh* lMesh = pMeshNode->GetMesh();
			const int lVertexCount = lMesh->GetControlPointsCount();
			const int deformerCount = lMesh->GetDeformerCount( FbxDeformer::eVertexCache );
			const int shapeCount = lMesh->GetShapeCount();
			const int skinCount = lMesh->GetDeformerCount( FbxDeformer::eSkin );

			FbxVector4* lVertexArray = new FbxVector4[ lVertexCount ];
			memcpy( lVertexArray, lMesh->GetControlPoints(), lVertexCount * sizeof( FbxVector4 ) );

			const int lPolygonCount = lMesh->GetPolygonCount();
			const int normalCount = lMesh->GetElementNormalCount();
			FbxGeometryElementNormal* geometryElementNormal = lMesh->GetElementNormal();
			FbxLayerElement::EMappingMode mappingMode = geometryElementNormal->GetMappingMode();

			for( int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; lPolygonIndex++ )
			{
				// We have already triengulated the data, this this has to be 3
				assert( lMesh->GetPolygonSize( lPolygonIndex ) == 3 );

				for( int lVerticeIndex = 0; lVerticeIndex < 3; lVerticeIndex++ )
				{
					int ctrlPointIndex = lMesh->GetPolygonVertex( lPolygonIndex, lVerticeIndex );

					FbxVector4 position;
					position = lMesh->GetControlPointAt( ctrlPointIndex ).mData;
					position[ 0 ] = lMesh->GetControlPointAt( ctrlPointIndex ).mData[ 0 ];
					position[ 1 ] = lMesh->GetControlPointAt( ctrlPointIndex ).mData[ 1 ];
					position[ 2 ] = lMesh->GetControlPointAt( ctrlPointIndex ).mData[ 2 ];
					position[ 3 ] = lMesh->GetControlPointAt( ctrlPointIndex ).mData[ 3 ];;

					FbxVector4 normal;
					normal[ 0 ] = geometryElementNormal->GetDirectArray().GetAt( ctrlPointIndex ).mData[ 0 ];
					normal[ 1 ] = geometryElementNormal->GetDirectArray().GetAt( ctrlPointIndex ).mData[ 1 ];
					normal[ 2 ] = geometryElementNormal->GetDirectArray().GetAt( ctrlPointIndex ).mData[ 2 ];
					normal[ 3 ] = 0;
				}
			}

			FbxLayerElement::EReferenceMode referenceMode = geometryElementNormal->GetReferenceMode();

			FBXSDK_printf( "Break here fucker!\n" );
		}
	}

    return 0;
}

