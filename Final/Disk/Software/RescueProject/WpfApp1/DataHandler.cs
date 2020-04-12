using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;

namespace WpfApp1
{
    public class DataHandler
    {
        public DataHandler()
        {
            tracks = new List<List<Point>>();
            times = new List<List<double>>();
            nVerticesInCluster = new List<int>();
            currTrack = new List<Point>();
            currTimes = new List<double>();
            nVerticesInCurrCluster = 0;
            t = 0;
        }
        
        public List<List<Point>> tracks;
        public List<List<double>> times;
        public List<int> nVerticesInCluster;
        public List<Point> currTrack;
        public List<double> currTimes;
        public int nVerticesInCurrCluster;
        public double t;
    }

}
