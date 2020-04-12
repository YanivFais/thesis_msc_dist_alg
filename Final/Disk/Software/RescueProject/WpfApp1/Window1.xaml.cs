using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;


namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class Window1 : Window
    {
        public Window1()
        {
            InitializeComponent();
            tracksData = new DataHandler();
            firstInTrack = true;
            duringTrack = false;
            pDown = new Point();
            initialSize = new Size(5, 5);
            button1.IsEnabled = false;
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            button1_ClickLogic();
        }

        private void button1_ClickLogic()
        {
            Window2 wnd = new Window2(tracksData);
            wnd.Owner = this;
            wnd.ShowDialog();

            if (tracksData.currTrack.Count > 0)
            {
                tracksData.tracks.Add(new List<Point>(tracksData.currTrack));
                tracksData.times.Add(new List<double>(tracksData.currTimes));
                tracksData.nVerticesInCluster.Add(tracksData.nVerticesInCurrCluster);
            }
            canvas1.Children.Clear();
            tracksData.currTrack.Clear();
            tracksData.currTimes.Clear();
            tracksData.nVerticesInCurrCluster = 0;
            button1.IsEnabled = false;
            
            firstInTrack = true;
            duringTrack = false;
            textBox1.Clear();
            tracksData.t = 0;

            for (int i = 0; i < tracksData.tracks.Count; i++)
            {
                List<Point> currTrack = tracksData.tracks[i];
                List<double> currTimes = tracksData.times[i];
                PointCollection trackPointCollection = new PointCollection();
                for (int j = 0; j < currTrack.Count; j++)
                {
                    Ellipse el = new Ellipse();
                    el.Visibility = System.Windows.Visibility.Visible;
                    el.Width = 8;
                    el.Height = 8;
                    el.HorizontalAlignment = System.Windows.HorizontalAlignment.Center;
                    el.VerticalAlignment = System.Windows.VerticalAlignment.Center;
                    Point tl = new Point(currTrack[j].X * canvas1.RenderSize.Width, currTrack[j].Y * canvas1.RenderSize.Height);
                    el.Margin = new Thickness(tl.X-4, tl.Y-4, 0, 0);
                    el.Fill = Brushes.Gray;
                    canvas1.Children.Add(el);
                    trackPointCollection.Add(tl);
                    Label lb = new Label();
                    lb.FontSize = 10;
                    lb.Visibility = System.Windows.Visibility.Visible;
                    lb.FontFamily = new System.Windows.Media.FontFamily("Thoma");
                    lb.Content = currTimes[j].ToString();
                    lb.Foreground = Brushes.Gray;
                    lb.Width = 20;
                    lb.Height = 20;
                    lb.Focusable = false;
                    lb.IsTabStop = false;
                    lb.Margin = new Thickness(tl.X-10, tl.Y - 20, 0, 0);
                    canvas1.Children.Add(lb);
                    Polyline lin1 = new Polyline();
                    lin1.Points = trackPointCollection;
                    lin1.Stroke = System.Windows.Media.Brushes.SlateGray;
                    lin1.StrokeThickness = 1;
                    canvas1.Children.Add(lin1);
                }
            }

        }

        public DataHandler tracksData;
        bool firstInTrack;
        bool duringTrack;

        Point pDown;
        Size initialSize;

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (duringTrack)
            {
                button1_ClickLogic();
            }
            WriteToFile();
        }

        private void WriteToFile()
        {
            int n = tracksData.tracks.Count;
            if (n == 0)
                return;

            StreamWriter outFile = new StreamWriter("points_metadata.txt");
            for (int i = 0; i < n; i++)
            {
                List<Point> currTrack = tracksData.tracks[i];
                List<double> currTimes = tracksData.times[i];
                int m = currTrack.Count;
                String str1;
                str1 = i.ToString() + "," + tracksData.nVerticesInCluster[i] + "," + m.ToString();
                outFile.WriteLine(str1);
                for (int j = 0; j < m; j++)
                {
                    String str;
                    str = j.ToString() + ",";
                    str = str + currTimes[j].ToString();
                    str = str + "," + currTrack[j].ToString();
                    outFile.WriteLine(str);
                }
            }
            outFile.Close();
        }

        private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            pDown = e.GetPosition(canvas1);
            pDown.X = Math.Max(Math.Min(pDown.X, canvas1.RenderSize.Width), 0);
            pDown.Y = Math.Max(Math.Min(pDown.Y, canvas1.RenderSize.Height), 0);
            Ellipse el = new Ellipse();
            el.Visibility = System.Windows.Visibility.Visible;
            el.Width = 5;
            el.Height = 5;
            Point tl = new Point(pDown.X - 5, pDown.Y - 5);
            el.Margin = new Thickness(tl.X, tl.Y, 0, 0);
            el.Fill = Brushes.Blue;
            textBox1.Clear();
            Point pNorm =
                new Point(Math.Round(pDown.X / canvas1.RenderSize.Width * 10000) / 10000, Math.Round(pDown.Y / canvas1.RenderSize.Height * 10000) / 10000);
            textBox1.AppendText(pNorm.ToString());
            canvas1.Children.Add(el);
            tracksData.currTrack.Add(new Point(pNorm.X, pNorm.Y));
            duringTrack = true;
            button1.IsEnabled = true;
            if (firstInTrack)
            {
                tracksData.t = 0;
                tracksData.currTimes.Add(tracksData.t);
            }
            else
            {
                Window3 timeWin = new Window3(tracksData);
                timeWin.Owner = this;
                timeWin.ShowDialog();
            }
            firstInTrack = false;

        }


    }
}
