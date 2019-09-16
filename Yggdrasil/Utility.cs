using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Yggdrasil
{
	class Utility
	{
		// Returns the distance in meters between two global coordinates
		// Based https://en.wikipedia.org/wiki/Haversine_formula
		public static double GlobalCoordinateToMeters(float lat1, float lon1, float lat2, float lon2)
		{
			// One degree in radians
			const double oneDegree = Math.PI / 180;

			// distance between latitudes and longitudes 
			double dLat = oneDegree * (lat2 - lat1);
			double dLon = oneDegree * (lon2 - lon1);

			// convert to radians 
			double lat1Radians = oneDegree * (lat1);
			double lat2Radians = oneDegree * (lat2);

			// apply formulae 
			double a = Math.Pow(Math.Sin(dLat / 2), 2) + Math.Pow(Math.Sin(dLon / 2), 2) * Math.Cos(lat1Radians) * Math.Cos(lat2Radians);

			return 12742000 * Math.Asin(Math.Sqrt(a));
		}

        // For now we will work with the following max size limitation on map data
        // 50 km * 50 km = 2,500,000,000 meters squared
        // 2.5 billion * 2 bytes (16 bit height values) = 5 billion bytes = 4,882,812KB = 4,768MB = 4.0GB

        // 50 km * 50 km using 10 meters per pixel => 5000 pixels * 5000 pixels
        public const int MetersPerPixel = 10;

        // If I made the limitation to be 131.072km x 131.072km square (131,072 meters by 131,072 meters)
        // 17,179,869,184 (~17 billion) meters squared at 1 pixel per meter resolution
        // 10 billion * 2 bytes (16 bit height values) = 34,359,738,368 bytes = 33,554,432KB = 32,768MB = 32.0GB

        // Max displayable image size could be 4096 x 4096
        // At 16bit gray scale the file size is approximately 32MB

        // Need to think about maybe using temp cache files
        // Each file will be a hexagon?
        // Editor will page them in and out of memory?

        // Current max resolution data from Terrain Party is 8km x 8km in a 1081 x 1081 pixel image
        // This is 7.4 meters per pixel

        // Chunk size thoughts
        // 1024x1024 images would be 2MB chunks in memory

        // ***** Zoom Levels *****
        // 100% 4096 meters x 4096 meters
        //  50% 8192 meters x 8192 meters
        //  25% 16,384 meters x 16,384 meters
        // ~12% 32,768 meters x 32,768 meters
        //  ~6% 65,536 meters x 65,536 meters
        //  ~3% 131,072 meters x 131,072 meters

        // ***** Hypothetical run through *****
        // After scanning the extents of all branches, we have a max limit area (131,072 meters by 131,072 meters)
        // The total data to be generated would be 32.0GB

        // We would then begin to generate 1024x1204 chunks
        // This would be multi threaded
        // Each thread would request a list of branches required for coverage of the chunk it is generating
        // Each chuck would be written to temporary disk space
        // We would build a grid that held the temp file names of each element

        // We might generate the top level view of the data and always keep that cached in memory
        // This would be a 4096x4096 image where each pixel would be 32 meters

        // A loading system would be capable of being given a filename and a resolution

        // There would be a loading system
        // When all branches are initially loaded
    }
}
