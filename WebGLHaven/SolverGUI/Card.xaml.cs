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
      public Card()
      {
         // normal component initialization
         InitializeComponent();

         // add event handlers
         KeyUp += Card_KeyUp;
      }

      public event EventHandler Tab;

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
               break;

            case Key.C:
            case Key.D:
            case Key.H:
            case Key.S:
               suit.Content = e.Key.ToString().ToUpper();
               break;

            case Key.Tab:
               Tab?.Invoke(this, EventArgs.Empty);
               break;
         }
      }

      public void TakeFocus()
      {
         Visibility = Visibility.Visible;
         Keyboard.Focus(this);
      }
   }
}
