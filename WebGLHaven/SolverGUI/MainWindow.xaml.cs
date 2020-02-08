using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace SolverGUI
{
   /// <summary>
   /// Interaction logic for MainWindow.xaml
   /// </summary>
   public partial class MainWindow : Window
   {
      private List<CardStack> columns = new List<CardStack>();

      public MainWindow()
      {
         // normal component initialization
         InitializeComponent();

         // make a handy list of our columns
         foreach (var child in columnsStack.Children)
            if (child is CardStack)
               columns.Add((CardStack)child);

         // add event handlers
         Loaded += MainWindow_Loaded;
      }

      private void MainWindow_Loaded(object sender, RoutedEventArgs e)
      {
         columns[0].TakeFocus();
      }
   }
}
