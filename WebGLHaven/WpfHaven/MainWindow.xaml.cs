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
      private SolverStub solver = new SolverStub();

      /// <summary>
      /// Test class for testing JavaScript integration
      /// </summary>
      public class SolverStub
      {
         /// <summary>
         /// bleems
         /// </summary>
         public string solve(string s)
         {
            Solver.NET.ManagedSolver solver = new Solver.NET.ManagedSolver();
            solver.Solve();
            return "Solver in the house";
         }
      }

      /// <summary>
      /// Initializes a new instance of class MainWindow
      /// </summary>
      public MainWindow()
      {
         // normal component initialization
         InitializeComponent();

         // add event handlers
         solveItem.Click += SolveItem_Click;
         webBrowser.FrameLoadEnd += WebBrowser_FrameLoadEnd;
         webBrowser.JavascriptObjectRepository.ResolveObject += JavascriptObjectRepository_ResolveObject;

         // navigate
         webBrowser.Address = new Uri(server.Url, "WebGLHaven/index.html").AbsoluteUri;
      }

      private void SolveItem_Click(object sender, RoutedEventArgs e)
      {
         var frame = WebBrowserExtensions.GetMainFrame(webBrowser);
         if (frame != null)
            frame.ExecuteJavaScriptAsync("game.solve(solver);");
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
