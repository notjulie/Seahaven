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
      private int activeCardCount = 0;

      public CardStack()
      {
         // normal component initialization
         InitializeComponent();

         // make a handy list of our columns
         foreach (var child in cardsStack.Children)
         {
            Card card = child as Card;
            if (card != null)
            {
               cards.Add((Card)child);
               card.Tab += Card_Tab;
            }
         }
      }

      private void Card_Tab(object sender, EventArgs e)
      {
         ++activeCardCount;
         TakeFocus();
      }

      public void TakeFocus()
      {
         cards[activeCardCount].TakeFocus();
      }
   }
}
