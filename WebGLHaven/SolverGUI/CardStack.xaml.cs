﻿using System;
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
      private int column = 0;

      /// <summary>
      /// Initializes a new instance of class CardStack.
      /// </summary>
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

         // initial state
         UpdateCardLocations();
      }

      /// <summary>
      /// Event fired when the user tabs out of the column
      /// </summary>
      public event EventHandler Tab;

      /// <summary>
      /// Gets or sets the column number
      /// </summary>
      public int Column
      {
         get
         {
            return column;
         }
         set
         {
            column = value;
            UpdateCardLocations();
         }
      }

      /// <summary>
      /// Gets the cards and locations
      /// </summary>
      public CardAndLocation[] CardsAndLocations
      {
         get
         {
            List<CardAndLocation> result = new List<CardAndLocation>();
            foreach (var card in cards)
               if (!card.IsEmpty)
                  result.Add(card.CardAndLocation);
            return result.ToArray();
         }
      }

      /// <summary>
      /// Responds to a tab when a card is selected
      /// </summary>
      /// <param name="sender"></param>
      /// <param name="e"></param>
      private void Card_Tab(object sender, EventArgs e)
      {
         // get the card
         Card card = (Card)sender;

         // get its index
         int index = cards.IndexOf(card);

         // if the card is empty, we hide it and move to the next column
         if (card.IsEmpty)
         {
            card.Visibility = Visibility.Hidden;
            Tab?.Invoke(this, EventArgs.Empty);
            return;
         }

         // if it's the last card then we tab to the next column
         if (index == cards.Count - 1)
         {
            Tab?.Invoke(this, EventArgs.Empty);
            return;
         }

         // else we just focus the next card
         cards[index + 1].TakeFocus();
      }

      /// <summary>
      /// Sets the focus to this control
      /// </summary>
      public void TakeFocus()
      {
         // set the focus to the first card
         cards[0].TakeFocus();
      }

      private void UpdateCardLocations()
      {
         for (int i = 0; i < cards.Count; ++i)
            cards[i].Location = "C" + Column + "-" + i;
      }
   }
}
