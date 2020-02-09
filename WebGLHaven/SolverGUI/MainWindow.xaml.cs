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

      /// <summary>
      /// Initializes a new instance of class MainWindow.
      /// </summary>
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
         solveItem.Click += SolveItem_Click;
         tower0.Tab += Tower0_Tab;
         tower1.Tab += Tower1_Tab;
         tower2.Tab += Tower2_Tab;
         tower3.Tab += Tower3_Tab;
      }

      private void Tower0_Tab(object sender, EventArgs e)
      {
         tower1.TakeFocus();
      }

      private void Tower1_Tab(object sender, EventArgs e)
      {
         tower2.TakeFocus();
      }

      private void Tower2_Tab(object sender, EventArgs e)
      {
         tower3.TakeFocus();
      }

      private void Tower3_Tab(object sender, EventArgs e)
      {
         column0.TakeFocus();
      }

      private void SolveItem_Click(object sender, RoutedEventArgs e)
      {
         // turn the cards into JSON; it's just a simple format that relates
         // cards to locations, such as:
         //   "JS" : "C2-1",
         // etc
         // start with the list...
         List<CardAndLocation> cardLocations = new List<CardAndLocation>();
         if (!tower0.IsEmpty)
            cardLocations.Add(tower0.CardAndLocation);
         if (!tower1.IsEmpty)
            cardLocations.Add(tower1.CardAndLocation);
         if (!tower2.IsEmpty)
            cardLocations.Add(tower2.CardAndLocation);
         if (!tower3.IsEmpty)
            cardLocations.Add(tower3.CardAndLocation);
         for (int i = 0; i < columns.Count; ++i)
            cardLocations.AddRange(columns[i].CardsAndLocations);

         StringBuilder s = new StringBuilder();
         s.Append("{");
         for (int i= 0; i < cardLocations.Count; ++i)
         {
            if (i != 0)
               s.Append(",");
            s.Append(cardLocations[i]);
         }
         s.Append("}");

         var solver = new ManagedSolver.ManagedSolver();
         string result = solver.Solve(s.ToString());
         MessageBox.Show(result);
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
