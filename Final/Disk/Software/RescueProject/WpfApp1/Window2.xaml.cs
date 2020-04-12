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
    /// Interaction logic for Window2.xaml
    /// </summary>
    public partial class Window2 : Window
    {
        public Window2(DataHandler tracksData)
        {
            InitializeComponent();
            trData = tracksData;
            dataEntered = false;
        }

        bool dataEntered;
        DataHandler trData;

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
            {
                bool valueOk = true;
                int N = 0;
                try
                {
                    N = System.Convert.ToInt32(this.textBox1.Text);
                }
                catch (System.FormatException)
                {
                    this.textBox1.Text = "";
                    valueOk = false;
                }
                if (valueOk)
                {
                    if ( (N > 0) && (N <= 100) )
                    {
                        trData.nVerticesInCurrCluster = N;
                        dataEntered = true;
                        this.Close();
                    }
                    else
                    {
                        this.textBox1.Text = "";
                        valueOk = false;
                    }
                }
                if (!valueOk)
                {
                    string messageBoxText = "Number of stations in cluster must be integer between 1 and 100";
                    string caption = "Error";
                    MessageBoxImage icon = MessageBoxImage.Error;
                    MessageBoxButton button = MessageBoxButton.OK;
                    MessageBox.Show(messageBoxText, caption, button, icon);
                }
                return;
            }

            if ((e.Key < Key.D0) || (e.Key > Key.D9))
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
