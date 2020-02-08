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
   /// Interaction logic for CardStack.xaml
   /// </summary>
   public partial class CardStack : UserControl
   {
      private List<Card> cards = new List<Card>();

      public CardStack()
      {
         // normal component initialization
         InitializeComponent();

         // make a handy list of our columns
         foreach (var child in cardsStack.Children)
            if (child is Card)
               cards.Add((Card)child);

      }

      public void TakeFocus()
      {
         cards[0].Visibility = Visibility.Visible;
      }
   }
}
