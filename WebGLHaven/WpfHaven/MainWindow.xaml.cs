using CefSharp;
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

namespace WpfHaven
{
   /// <summary>
   /// Interaction logic for MainWindow.xaml
   /// </summary>
   public partial class MainWindow : Window
   {
      private Server server = new Server();
      private Solver solver = new Solver();

      public class Solver
      {
         public void dobleem()
         {

         }
      }

      public MainWindow()
      {
         InitializeComponent();

         // add event handlers
         webBrowser.FrameLoadEnd += WebBrowser_FrameLoadEnd;
         webBrowser.JavascriptObjectRepository.ResolveObject += JavascriptObjectRepository_ResolveObject;

         // navigate
         webBrowser.Address = new Uri(server.Url, "WebGLHaven/index.html").AbsoluteUri;
      }

      private void WebBrowser_FrameLoadEnd(object sender, FrameLoadEndEventArgs e)
      {
         // execute some JavaScript that will install our Solver object in the newly loaded frame
         var solverBinder = new StringBuilder();
         solverBinder.Append("(async () =>")
         .Append("{")
         .Append("await CefSharp.BindObjectAsync('solver');")
         .Append("})();");
         e.Frame.ExecuteJavaScriptAsync(solverBinder.ToString());
      }

      private void JavascriptObjectRepository_ResolveObject(object sender, CefSharp.Event.JavascriptBindingEventArgs e)
      {
         switch (e.ObjectName)
         {
            case "solver":
               e.ObjectRepository.Register("solver", solver, true);
               break;
         }
      }
   }
}
