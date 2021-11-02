using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CablesWindoApp3.Classes
{
    
    public class XP
    {
        public float OuterDia;
        public float avgThick;
        public float concentricity;
        public float minThick;
        public List<float> minThickData = new List<float>();

        public XP(float OuterDia, float avgThick, float concentricity, float minThick, List<float> minThickData)
        {
            this.OuterDia = OuterDia;
            this.avgThick = avgThick;
            this.concentricity = concentricity;
            this.minThick = minThick;
            this.minThickData = minThickData;
        }

        public List<XP> xlpes = new List<XP>();

        public XP()
        {
            xlpes = new List<XP>();
        }
    }
    
}
