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
using System.Windows.Threading;

namespace SolverGUI
{
   /// <summary>
   /// Interaction logic for Card.xaml
   /// </summary>
   public partial class Card : UserControl
   {
      #region Constructor

      /// <summary>
      /// Initializes a new instance of class Card
      /// </summary>
      public Card()
      {
         // normal component initialization
         InitializeComponent();

         // get rid of the focus visual style... the automatic focus highlight has weird
         // rules so that it doesn't show unless it is focused in response to a key event
         FocusVisualStyle = null;

         // add event handlers
         PreviewKeyDown += Card_PreviewKeyDown;
         PreviewKeyUp += Card_PreviewKeyUp;
         KeyUp += Card_KeyUp;
         MouseUp += Card_MouseUp;
         IsKeyboardFocusedChanged += Card_IsKeyboardFocusedChanged;
      }

      #endregion

      #region Events / Properties

      /// <summary>
      /// Event fired when tab key is pressed
      /// </summary>
      public event EventHandler Tab;

      /// <summary>
      /// Gets or sets the location associated with this card
      /// </summary>
      public string Location
      {
         get;
         set;
      }

      /// <summary>
      /// Gets the card and location string
      /// </summary>
      public CardAndLocation CardAndLocation
      {
         get
         {
            CardAndLocation result = new CardAndLocation();
            result.card = Rank + Suit;
            result.location = Location;
            return result;
         }
      }

      /// <summary>
      /// Gets the rank
      /// </summary>
      public string Rank
      {
         get
         {
            if (rank.Content == null)
               return string.Empty;
            else
               return rank.Content.ToString();
         }
      }

      /// <summary>
      /// Gets the suit
      /// </summary>
      public string Suit
      {
         get
         {
            if (suit.Content == null)
               return string.Empty;
            else
               return suit.Content.ToString();
         }
      }

      /// <summary>
      /// Gets a value indicating whether the card's user input fields are empty
      /// </summary>
      public bool IsEmpty
      {
         get
         {
            return Rank.Length == 0 && Suit.Length == 0;
         }
      }

      #endregion

      #region Public Methods

      /// <summary>
      /// Focuses the card
      /// </summary>
      public void TakeFocus()
      {
         Visibility = Visibility.Visible;
         Keyboard.Focus(this);
         Focus();
      }

      #endregion

      #region Event Handlers

      private void Card_IsKeyboardFocusedChanged(object sender, DependencyPropertyChangedEventArgs e)
      {
         borderHighlight.Visibility = IsKeyboardFocused ? Visibility.Visible : Visibility.Hidden;
      }

      private void Card_MouseUp(object sender, MouseButtonEventArgs e)
      {
         TakeFocus();
      }

      private void Card_KeyUp(object sender, KeyEventArgs e)
      {
         switch (e.Key)
         {
            case Key.A:
            case Key.D2:
            case Key.D3:
            case Key.D4:
            case Key.D5:
            case Key.D6:
            case Key.D7:
            case Key.D8:
            case Key.D9:
            case Key.T:
            case Key.J:
            case Key.Q:
            case Key.K:
               rank.Content = (char)KeyInterop.VirtualKeyFromKey(e.Key);
               e.Handled = true;
               break;

            case Key.C:
            case Key.D:
            case Key.H:
            case Key.S:
               suit.Content = e.Key.ToString().ToUpper();
               e.Handled = true;
               break;
         }
      }

      private void Card_PreviewKeyDown(object sender, KeyEventArgs e)
      {
         switch (e.Key)
         {
            case Key.Tab:
               e.Handled = true;
               break;
         }
      }

      /// <summary>
      /// Handles key events that we need to make sure nobody else tries
      /// to intercept; yes, I mean tab
      /// </summary>
      /// <param name="sender"></param>
      /// <param name="e"></param>
      private void Card_PreviewKeyUp(object sender, KeyEventArgs e)
      {
         switch (e.Key)
         {
            case Key.Tab:
               Tab?.Invoke(this, EventArgs.Empty);
               e.Handled = true;
               break;
         }
      }

      #endregion
   }
}
