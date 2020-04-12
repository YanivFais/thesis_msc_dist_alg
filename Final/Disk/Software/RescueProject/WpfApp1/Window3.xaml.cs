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
using System.Windows.Shapes;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for Window3.xaml
    /// </summary>
    public partial class Window3 : Window
    {
        public Window3(DataHandler tracksData)
        {
            InitializeComponent();
            trData = tracksData;
            dataEntered = false;
        }

        DataHandler trData;
        bool dataEntered;

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return) 
            {
                bool valueOk = true;
                double tt = 0;
                try
                {
                    tt = System.Convert.ToDouble(this.textBox1.Text);
                }
                catch (System.FormatException)
                {
                    this.textBox1.Text = "";
                    valueOk = false;
                }
                if (valueOk)
                {
                    if ((tt > 1000) || (tt < 0))
                    {
                        string messageBoxText = "Time must be integer between 0 and 1000";
                        string caption = "Error";
                        MessageBoxImage icon = MessageBoxImage.Error;
                        MessageBoxButton button = MessageBoxButton.OK;
                        MessageBox.Show(messageBoxText, caption, button, icon);
                    }
                    else
                    if (tt <= trData.t)
                    {
                        string messageBoxText = "New point time must be bigger than previous points time";
                        string caption = "Error";
                        MessageBoxImage icon = MessageBoxImage.Error;
                        MessageBoxButton button = MessageBoxButton.OK;
                        MessageBox.Show(messageBoxText, caption, button, icon);
                    }
                    else
                    {
                        trData.t = tt;
                        trData.currTimes.Add(tt);
                        dataEntered = true;
                        this.Close();
                    }
                }
                return;
            }
                
            if ((e.Key < Key.D0) || (e.Key > Key.D9) )
            {
                e.Handled = true;
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (!dataEntered)
                e.Cancel = true;

        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            FocusManager.SetFocusedElement(textBox1, textBox1);
                
                //.FocusedElement="{Binding ElementName=myTextBox}"
        }

    }
}
