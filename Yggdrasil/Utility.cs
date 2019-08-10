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
	}
}
