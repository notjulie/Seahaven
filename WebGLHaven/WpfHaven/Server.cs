using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Mime;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace WpfHaven
{
   /// <summary>
   /// Internal http server that provides the content for our browser control
   /// from embedded resources
   /// </summary>
   public class Server
   {
      string _Html = String.Empty;
      string _BaseDirectory = String.Empty;

      /// <summary>
      /// Gets our base URL
      /// </summary>
      public Uri Url
      {
         get
         {
            return new Uri(
"http://" + "localhost" + ":" + _Port + "/");
         }
      }

      HttpListener _Listener = null;
      int _Port = -1;

      /// <summary>
      /// Initializes a new instance of class Server
      /// </summary>
      public Server()
      {
         var rnd = new Random();

         for (int i = 0; i < 100; i++)
         {
            int port = rnd.Next(49152, 65536);

            try
            {
               _Listener = new HttpListener();
               _Listener.Prefixes.Add("http://localhost:" + port + "/");
               _Listener.Start();

               _Port = port;
               _Listener.BeginGetContext(ListenerCallback, null);
               return;
            }
            catch (Exception x)
            {
               _Listener.Close();
               Debug.WriteLine("HttpListener.Start:\n" + x);
            }
         }

         throw new ApplicationException("Failed to start HttpListener");
      }

      /// <summary>
      /// Callback that gets called to respond to an HTTP request
      /// </summary>
      /// <param name="ar"></param>
      public void ListenerCallback(IAsyncResult ar)
      {
         _Listener.BeginGetContext(ListenerCallback, null);

         var context = _Listener.EndGetContext(ar);
         var request = context.Request;
         var response = context.Response;

         Debug.WriteLine("SERVER: " + _BaseDirectory + " " + request.Url);

         response.AddHeader("Cache-Control", "no-cache");

         try
         {
            if (request.Url.AbsolutePath == "/")
            {
               response.ContentType = MediaTypeNames.Text.Html;
               response.ContentEncoding = Encoding.UTF8;

               var buffer = Encoding.UTF8.GetBytes(_Html);
               response.ContentLength64 = buffer.Length;
               using (var s = response.OutputStream) s.Write(buffer, 0, buffer.Length);

               return;
            }

            // get the resource stream associated with the URL
            Stream resourceStream;
            string resourceName = "WpfHaven" + request.Url.AbsolutePath.Replace('/', '.');
            try
            {
               Assembly assembly = Assembly.GetExecutingAssembly();
               var streamNames = assembly.GetManifestResourceNames();
               resourceStream = assembly.GetManifestResourceStream(resourceName);
               if (resourceStream == null)
                  throw new Exception("No such resource: " + resourceName);
            }
            catch (Exception x)
            {
               Debug.WriteLine(x.Message);
               response.StatusCode = (int)HttpStatusCode.NotFound; // 404
               response.StatusDescription = response.StatusCode + " Not Found";

               response.ContentType = MediaTypeNames.Text.Html;
               response.ContentEncoding = Encoding.UTF8;

               var buffer = Encoding.UTF8.GetBytes(
                 "<html><body>404 Not Found</body></html>");
               response.ContentLength64 = buffer.Length;
               using (var s = response.OutputStream) s.Write(buffer, 0, buffer.Length);

               return;
            }

            response.ContentLength64 = resourceStream.Length;

            switch (Path.GetExtension(request.Url.AbsolutePath).ToLowerInvariant())
            {
               //images
               case ".gif": response.ContentType = MediaTypeNames.Image.Gif; break;
               case ".jpg":
               case ".jpeg": response.ContentType = MediaTypeNames.Image.Jpeg; break;
               case ".tiff": response.ContentType = MediaTypeNames.Image.Tiff; break;
               case ".png": response.ContentType = "image/png"; break;

               // application
               case ".pdf": response.ContentType = MediaTypeNames.Application.Pdf; break;
               case ".zip": response.ContentType = MediaTypeNames.Application.Zip; break;

               // text
               case ".htm":
               case ".html": response.ContentType = MediaTypeNames.Text.Html; break;
               case ".txt": response.ContentType = MediaTypeNames.Text.Plain; break;
               case ".xml": response.ContentType = MediaTypeNames.Text.Xml; break;
               case ".js": response.ContentType = "text/javascript"; break;
               case ".css": response.ContentType = "text/css"; break;

               // let the user agent work it out
               default: response.ContentType = MediaTypeNames.Application.Octet; break;
            }

            // send the stream contents
            using (var s = response.OutputStream)
               resourceStream.CopyTo(s);
         }
         catch (Exception x)
         {
            Debug.WriteLine("Unexpected exception. Aborting...\n" + x);

            response.Abort();
         }
      }
   }
}
