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

        public bool Load(string filePath)
        {
            m_Width = 0;
            m_Height = 0;
            m_Image = null;
            m_FilePath = "";

            if ( File.Exists( filePath ) )
            {
                //reading from the file
                try
                {
                    using ( Stream inputStream = new FileStream( filePath, FileMode.Open, FileAccess.Read, FileShare.Read ) )
                    {
                        m_Image = Image.FromStream( inputStream );

                        m_Width = m_Image.Width;
                        m_Height = m_Image.Height;

                        m_FilePath = filePath;
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

        private string m_FilePath = "";
        private int m_Width;
        private int m_Height;
        private Image m_Image = null;
    }
}
