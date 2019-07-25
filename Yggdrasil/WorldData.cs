using ImageMagick;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Yggdrasil
{
    class WorldData
    {
        public string FilePath { get { return m_FilePath; } }
        public int Width { get { return m_Width; } }
        public int Height { get { return m_Height; } }
        public Image Image { get { return m_Image; } }

        // Load a yggdrasil file.
        public bool Load(string filePath)
        {
            // Erases all loaded data.

            // The data loaded will be a table of data elements that will be used 
            // to generate a live copy of world data

            m_Width = 0;
            m_Height = 0;
            m_Image = null;
            m_FilePath = "";

            if ( File.Exists( filePath ) )
            {
                //reading from the file
                try
                {
                    using (MagickImage image = new MagickImage(filePath))
                    {
                        int bitDepth = image.BitDepth();

                        m_Width = image.Width;
                        m_Height = image.Height;

                        m_Image = image.ToBitmap();

                        if (bitDepth == 8)
                        {
                            Console.WriteLine("8 Bit");
                        }
                    }

                    return true;
                }
                catch ( Exception )
                {
                    // Catch everything and ignore
                }
            }

            return false;
        }

        // Save a yggdrasil file
        public bool Save(string filePath)
        {
            return false;
        }

        // Import a single image, merging it into the current world data
        public bool ImportImage(string filePath)
        {
            if (File.Exists(filePath))
            {
                // Attempt to import the given file as image data
                try
                {
                    using (MagickImage image = new MagickImage(filePath))
                    {
                        // Perform any extra verification here?

                        // The image has been loaded

                        // Extract the needed information

                        // Add the image to our yggdrasil data

                        int bitDepth = image.BitDepth();

                        m_Width = image.Width;
                        m_Height = image.Height;

                        m_Image = image.ToBitmap();

                        if (bitDepth == 8)
                        {
                            Console.WriteLine("8 Bit");
                        }
                    }

                    return true;
                }
                catch (Exception)
                {
                    // Catch everything and ignore
                }
            }

            return false;
        }

        // Exports the world data in a binary format that contains no external data references
        public bool Export(string filePath)
        {
            return false;
        }

        private string m_FilePath = "";
        private int m_Width;
        private int m_Height;
        private Image m_Image = null;
    }
}
