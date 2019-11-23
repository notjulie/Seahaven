/*
* Project: iHaven
* Author: Randy Rasmussen
* Copyright: None
* Warranty: None
*/


import UIKit
import WebKit

class ViewController: UIViewController {
        
    var webView: WKWebView!
    
    var initialLoadAction: WKNavigation?
    
    override func viewDidLoad() {
      super.viewDidLoad()

      // Create a configuration, load any required scripts
      // and register custom schemes.
      let config = WKWebViewConfiguration()
      loadScripts(config: config)
      setUpSchemes(config: config)


      // Create a WKWebView and set its delegates
      webView = WKWebView(frame: self.view.frame, configuration: config)
      webView.navigationDelegate = self
      webView.uiDelegate = self

      // Set Up Auto Layout for the WKWebView
      webView.translatesAutoresizingMaskIntoConstraints = false
      view.addSubview(webView)

      // Load the initial file
      let url = URL(string: "dr-bundle-file:///index.html")!
      initialLoadAction = webView.load(URLRequest(url:url))

      // do the initial layout
      doLayout()
   }
    
    /// Load the custom script file from the bundle and add them to the
    /// WKWebViewConfiguration content controller.
    private func loadScripts(config: WKWebViewConfiguration) {
      /*
        // Get the paths for the two custom script files.
        guard let changeCSSFilePath = Bundle.main.path(forResource: "changeCSS", ofType: "js"),
            let filterTownsFilePath = Bundle.main.path(forResource: "filterTowns", ofType: "js")else {
                fatalError("Scripts Not Found")
        }

        do {
            // Add the change-css script file.
            let changeCSSJS = try String(contentsOfFile: changeCSSFilePath)
            let changeCSSUserScript = WKUserScript(source: changeCSSJS,
                                          injectionTime: .atDocumentEnd,
                                          forMainFrameOnly: true)
            config.userContentController.addUserScript(changeCSSUserScript)
            
            // Add the filterTowns script file.
            let filterTownsJS = try String(contentsOfFile: filterTownsFilePath)
            let filterTownsUserScript = WKUserScript(source: filterTownsJS,
                                                   injectionTime: .atDocumentEnd,
                                                   forMainFrameOnly: true)
            config.userContentController.addUserScript(filterTownsUserScript)
            
            // Register to receive populationHasChanged messages.
            config.userContentController.add(self, name: "populationHasChanged")
        } catch {
            fatalError("Something went wrong: \(error)")
        }*/
    }
    
    /// Register custom url scheme with the WKWebViewConfiguration.
    private func setUpSchemes(config: WKWebViewConfiguration) {
        config.setURLSchemeHandler(self, forURLScheme: "dr-bundle-file")
    }

   override func viewSafeAreaInsetsDidChange() {
      doLayout()
   }
   
   override func viewWillTransition(to size: CGSize, with coordinator: UIViewControllerTransitionCoordinator) {
        //positionViews(to: size);
      coordinator.animate(alongsideTransition: { (context) in
         // During rotation
      }) { [weak self] (context) in
         // After rotation
         self!.doLayout()
      }
   }
    
   private func doLayout() {
      self.webView!.frame = self.view.safeAreaLayoutGuide.layoutFrame
   }}

