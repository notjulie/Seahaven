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
         {
            CardStack column = child as CardStack;
            if (column != null)
            {
               columns.Add(column);
               column.Tab += Column_Tab;
            }
         }

         // add event handlers
         Loaded += MainWindow_Loaded;
      }

      private void Column_Tab(object sender, EventArgs e)
      {
         // focus the next column
         int index = columns.IndexOf((CardStack)sender);
         if (index < columns.Count - 1)
            columns[index + 1].TakeFocus();
         else
            tower0.TakeFocus();
      }

      private void MainWindow_Loaded(object sender, RoutedEventArgs e)
      {
         columns[0].TakeFocus();
      }
   }
}
