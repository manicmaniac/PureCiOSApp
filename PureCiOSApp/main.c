//
//  main.c
//  PureCiOSApp
//
//  Created by Ryosuke Ito on 6/17/20.
//  Copyright © 2020 Ryosuke Ito. All rights reserved.
//

#include <CoreGraphics/CoreGraphics.h>
#include <objc/message.h>
#include <objc/runtime.h>

#define objc_msgSend(T) ((T (*)(void *, SEL, ...))objc_msgSend)
#define objc_msgSendSuper(T) ((T (*)(struct objc_super *, SEL, ...))objc_msgSendSuper)
#define super (&(struct objc_super){ self, class_getSuperclass(object_getClass(self)) })

// MARK: - ViewController

static void ViewController_viewDidLoad(id self, SEL _cmd) {
    // [super viewDidLoad];
    objc_msgSendSuper(void)(super, sel_registerName("viewDidLoad"));

    // [[self view] setBackgroundColor:[UIColor greenColor]];
    id view = objc_msgSend(id)(self, sel_registerName("view"));
    Class UIColor = objc_getClass("UIColor");
    id green = objc_msgSend(id)(UIColor, sel_registerName("greenColor"));
    objc_msgSend(void)(view, sel_registerName("setBackgroundColor:"), green);

    // id imageView = [[[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 300, 300)] autorelease];
    Class UIImageView = objc_getClass("UIImageView");
    id imageView = objc_msgSend(id)(objc_msgSend(id)(objc_msgSend(id)(UIImageView, sel_registerName("alloc")), sel_registerName("initWithFrame:"), CGRectMake(0, 0, 300, 300)), sel_registerName("autorelease"));

    // [imageView setImage:[UIImage imageNamed:@"ferret"];
    Class UIImage = objc_getClass("UIImage");
    id image = objc_msgSend(id)(UIImage, sel_registerName("imageNamed:"), (id)CFSTR("ferret"));
    objc_msgSend(void)(imageView, sel_registerName("setImage:"), image);

    // [imageView setContentMode:UIViewContentModeScaleAspectFit];
    objc_msgSend(void)(imageView, sel_registerName("setContentMode:"), 1);

    // [imageView setTranslatesAutoresizingMaskIntoConstraints:NO];
    objc_msgSend(void)(imageView, sel_registerName("setTranslatesAutoresizingMaskIntoConstraints:"), NO);

    // [[self view] addSubview:imageView];
    objc_msgSend(void)(view, sel_registerName("addSubview:"), imageView);
}

static Class registerViewController(void) {
    // @implementation ViewController : UIViewController
    Class UIViewController = objc_getClass("UIViewController");
    Class ViewController = objc_allocateClassPair(UIViewController, "ViewController", 0);
    objc_registerClassPair(ViewController);

    // - (void)viewDidLoad;
    if (!class_addMethod(ViewController, sel_registerName("viewDidLoad"), (IMP)ViewController_viewDidLoad, "v@:")) {
        return NULL;
    }
    return ViewController;
}

// MARK: - AppDelegate

static void AppDelegate_applicationDidFinishLaunchingWithOptions(id self, SEL _cmd, id application, id options) {
    // id viewController = [[[ViewController alloc] init] autorelease];
    Class ViewController = registerViewController();
    id viewController = objc_msgSend(id)(objc_msgSend(id)(objc_msgSend(id)(ViewController, sel_registerName("alloc")), sel_registerName("init")), sel_registerName("autorelease"));

    // id window = [[UIWindow alloc] init];
    Class UIWindow = objc_getClass("UIWindow");
    id window = objc_msgSend(id)(objc_msgSend(id)(UIWindow, sel_registerName("alloc")), sel_registerName("init"));

    // [window setRootViewController:viewController];
    objc_msgSend(void)(window, sel_registerName("setRootViewController:"), viewController);

    // [window makeKeyAndVisible];
    objc_msgSend(void)(window, sel_registerName("makeKeyAndVisible"));

    // _window = window;
    object_setInstanceVariable(self, "_window", window);
}

static void AppDelegate_dealloc(id self, SEL _cmd) {
    // [_window release];
    id window = NULL;
    object_getInstanceVariable(self, "_window", (void *)(&window));
    objc_msgSend(void)(window, sel_registerName("release"));

    // [super dealloc];
    objc_msgSendSuper(void)(super, sel_registerName("dealloc"));
}

static Class registerAppDelegate(void) {
    // @implementation AppDelegate : UIResponder
    Class UIResponder = objc_getClass("UIResponder");
    Class AppDelegate = objc_allocateClassPair(UIResponder, "AppDelegate", 0);
    // id _window;
    if (!class_addIvar(AppDelegate, "_window", sizeof(id), 0, "@")) {
        return NULL;
    }
    // - (void)application:(id)application didFinishLaunchingWithOptions:(id)options;
    objc_registerClassPair(AppDelegate);
    if (!class_addMethod(AppDelegate, sel_registerName("application:didFinishLaunchingWithOptions:"), (IMP)AppDelegate_applicationDidFinishLaunchingWithOptions, "v@:@@")) {
        return NULL;
    }
    if (!class_addMethod(AppDelegate, sel_registerName("dealloc"), (IMP)AppDelegate_dealloc, "v@:")) {
        return NULL;
    }
    return AppDelegate;
}

// MARK: - main

extern int UIApplicationMain(int, char **, id, id);

int main(int argc, char * argv[]) {
    // id pool = [[NSAutoreleasePool alloc] init];
    Class NSAutoreleasePool = objc_getClass("NSAutoreleasePool");
    id pool = objc_msgSend(id)(objc_msgSend(id)(NSAutoreleasePool, sel_registerName("alloc")), sel_registerName("init"));

    registerAppDelegate();
    int status = UIApplicationMain(argc, argv, nil, (id)CFSTR("AppDelegate"));

    // [pool drain]; [pool release];
    objc_msgSend(void)(pool, sel_registerName("drain"));
    objc_msgSend(void)(pool, sel_registerName("release"));
    return status;
}
