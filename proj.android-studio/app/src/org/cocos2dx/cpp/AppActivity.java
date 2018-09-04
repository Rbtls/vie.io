/****************************************************************************
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2011      Zynga Inc.
 Copyright (c) 2013-2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.plugin.PluginWrapper;
import android.os.Bundle;
import android.os.Handler;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import android.support.v7.app.AppCompatActivity;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.vie.io.R;
import android.app.Activity;
import android.app.Application;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdRequest.Builder;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthCredential;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.auth.GoogleAuthProvider;
import android.support.annotation.NonNull;
import android.support.design.widget.Snackbar;
import android.support.multidex.MultiDex;
import android.support.test.InstrumentationRegistry;

import android.graphics.Color;
import android.graphics.Bitmap;
import android.graphics.PixelFormat;
import android.os.Bundle;
import android.os.Build;
import android.widget.FrameLayout;
import android.webkit.WebView;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebViewClient;
import android.webkit.SafeBrowsingResponse;
import android.webkit.SslErrorHandler;
import android.net.http.SslError;
import android.view.Gravity;
import android.view.View;
import android.util.Log;
import android.content.Intent;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import static com.google.android.gms.games.Games.getGamesClient;
//import static com.google.android.gms.internal.zzahn.runOnUiThread;

public class AppActivity extends Cocos2dxActivity {

	private String TAG = AppActivity.class.getSimpleName();
	private InterstitialAd mInterstitialAd;
	private static AdRequest _adRequest;
	private static AppActivity _appActivity;
	// Request codes for the UIs that we show with startActivityForResult:
    final static int RC_SELECT_PLAYERS = 10000;
    final static int RC_INVITATION_INBOX = 10001;
    final static int RC_WAITING_ROOM = 10002;
	private static final int RC_SIGN_IN = 9001;
	private AdView admobBannerAdView;
	
	private WebView webView;
	private volatile WebChromeClient webChromeClient;
    private ProgressDialog progDailog; 
    private FirebaseAuth mAuth;
	
	String url="http://www.teluguoneradio.com/rssHostDescr.php?hostId=147";
    
	@Override
	protected void onCreate(final Bundle savedInstanceState) {
		MultiDex.install(getApplicationContext());
		super.onCreate(savedInstanceState);
        _appActivity = this;
		nativeInitGPGS(this);
		mAuth = FirebaseAuth.getInstance();
		MobileAds.initialize(this, getString(R.string.app_id));
        setWebView();
		nativeOnActivityCreated(this, savedInstanceState);

        _adRequest = new AdRequest.Builder()
				.addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
				.addTestDevice(getString(R.string.ad_device)) //test_device
				.build();

		mInterstitialAd = new InterstitialAd(this);
		// Set the ad unit ID
		mInterstitialAd.setAdUnitId(getString(R.string.regular_interstitial));
		
		// Set an AdListener.
		mInterstitialAd.setAdListener(new AdListener() {

			@Override
			public void onAdLoaded() {
				mInterstitialAd.loadAd(_adRequest);
			}

			@Override
			public void onAdClosed() {
				mInterstitialAd.loadAd(_adRequest);
			}

			@Override
			public void onAdFailedToLoad(int errorCode) {
				mInterstitialAd.loadAd(_adRequest);
			}

			@Override
			public void onAdOpened() {
				mInterstitialAd.loadAd(_adRequest);
			}

		});
		// Load ads into Interstitial Ads
		//mInterstitialAd.loadAd(_adRequest);
		
		//Banner
		FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.WRAP_CONTENT);
		params.gravity = Gravity.BOTTOM;
		admobBannerAdView = new AdView(this);

		admobBannerAdView.setAdSize(AdSize.SMART_BANNER);
		admobBannerAdView.setAdUnitId(getString(R.string.banner_home_footer));
		admobBannerAdView.setBackgroundColor(Color.TRANSPARENT);
		admobBannerAdView.setPadding(0, 0, 0, 0);
		//admobBannerAdView.loadAd(_adRequest);

		addContentView(admobBannerAdView, params);

       // setContentView(R.layout.activity_main);
        //webView.loadUrl(url);
	}

        public Cocos2dxGLSurfaceView onCreateView() {
                Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
                // App should create stencil buffer
                glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
                glSurfaceView.getHolder().setFormat(PixelFormat.TRANSLUCENT);

                return glSurfaceView;
        } 

	static {
		System.loadLibrary("MyGame");
	}

	public static void showAdmobBannerAd() {
		_appActivity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				if (null == _appActivity.admobBannerAdView) {
					return;
				}
				if (!_appActivity.admobBannerAdView.isEnabled())
					_appActivity.admobBannerAdView.setEnabled(true);
				_appActivity.admobBannerAdView.setVisibility(View.VISIBLE);
				_appActivity.admobBannerAdView.loadAd(_adRequest);
			}
		});

	}

	public static void hideAdmobBannerAd() {
		_appActivity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				if (null == _appActivity.admobBannerAdView) {
					return;
				}

				_appActivity.admobBannerAdView.setEnabled(false);
				_appActivity.admobBannerAdView.setVisibility(View.INVISIBLE);
				_appActivity.admobBannerAdView.loadAd(_adRequest);
			}
		});
	}

	public static void showInterstitial() {
		_appActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if (null == _appActivity.mInterstitialAd) {
					return;
				}
				if (_appActivity.mInterstitialAd.isLoaded())
					_appActivity.onPause();
				_appActivity.mInterstitialAd.show();
				_appActivity.mInterstitialAd.loadAd(_adRequest);
			}
		});
	}

	private void setWebView() {
		webView = (WebView) findViewById(R.id.webView);
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                webView.getSettings().setSafeBrowsingEnabled(false);
        }
		webView.getSettings().setAppCachePath(getApplicationContext().getCacheDir().getAbsolutePath() );
        webView.getSettings().setAllowFileAccess( true );
        webView.getSettings().setAppCacheEnabled( true );
        webView.getSettings().setJavaScriptEnabled( true );
        webView.getSettings().setJavaScriptCanOpenWindowsAutomatically(true);
        webView.getSettings().setCacheMode( WebSettings.LOAD_DEFAULT );
		webView.setWebViewClient(new WebViewClient() {

			@Override
			public void onReceivedSslError(WebView view, SslErrorHandler handler, SslError error) {
				handler.proceed(); // Ignore SSL certificate errors
				// fix for: SSL Error. Failed to validate the certificate chain

			}

			@Override
			public void onPageStarted(WebView view, String url, Bitmap favicon) {
				//Do some like progress bar loading here.
				//mLayoutProgress.setVisibility(View.VISIBLE);
			}

			@Override
			public void onPageFinished(WebView view, String url) {

				//webView.loadUrl(sourceURL);
				//mLayoutProgress.setVisibility(View.GONE);
			}

			@Override
			public boolean shouldOverrideUrlLoading(WebView view, String url) {
				return false;
			}
		});
		webView.setWebChromeClient(new WebChromeClient());
	}

	/**
	 * Set the WebChromeClient.
	 * @param client An implementation of WebChromeClient.
	 */
	public void setWebChromeClient(WebChromeClient client) {
		webChromeClient = client;
	}

    private void firebaseAuthWithGoogle(GoogleSignInAccount acct) {
        Log.d(TAG, "firebaseAuthWithGoogle:" + acct.getId());

        AuthCredential credential = GoogleAuthProvider.getCredential(acct.getIdToken(), null);
        mAuth.signInWithCredential(credential)
                .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if (task.isSuccessful()) {
                            // Sign in success, update UI with the signed-in user's information
                            Log.d(TAG, "signInWithCredential:success");
                            FirebaseUser user = mAuth.getCurrentUser();
                            //updateUI(user);
                        } else {
                            // If sign in fails, display a message to the user.
                            Log.w(TAG, "signInWithCredential:failure", task.getException());
                            Snackbar.make(findViewById(R.id.main_layout), "Authentication Failed.", Snackbar.LENGTH_SHORT).show();
                            //updateUI(null);
                        }

                        // ...
                    }
                });
    }


	@Override
	public void setCocosView(Cocos2dxGLSurfaceView mGLSurfaceView) {

		setContentView(R.layout.activity_main);
		RelativeLayout framelayout = (RelativeLayout) findViewById(R.id.gameviewLay);
		framelayout.addView(mGLSurfaceView);
		System.out.println("txc:setCocosView");
	}

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
        nativeOnActivityResult(this, requestCode,resultCode, intent); 
    }

    // Show error message about game being cancelled and return to main screen.
    void showGameError() {
      new AlertDialog.Builder(this)
          .setMessage(getString(R.string.game_problem))
          .setNeutralButton(android.R.string.ok, null).create();
    }

    void keepScreenOn() {
      getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    // Clears the flag that keeps the screen on.
    void stopKeepingScreenOn() {
      getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

	@Override
	protected void onResume() {
		admobBannerAdView.resume();
		super.onResume();
		nativeOnActivityResumed(this);
		//signInSilently();
	}

	@Override
	protected void onPause() {
		admobBannerAdView.pause();
		super.onPause();

		nativeOnActivityPaused(this);
	}

	@Override
	protected void onDestroy() {

		if (null != _appActivity.admobBannerAdView) {
			admobBannerAdView.destroy();
		}

		super.onDestroy();
		nativeOnActivityDestroyed(this);
	}

	@Override
	protected void onSaveInstanceState(Bundle outState) {
		// TODO Auto-generated method stub
		super.onSaveInstanceState(outState);
		nativeOnActivitySaveInstanceState(this, outState);
	}


	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
		nativeOnActivityStarted(this);
		// Check if user is signed in (non-null) and update UI accordingly.
       // FirebaseUser currentUser = mAuth.getCurrentUser();
		//signInSilently();
	}


	@Override
	protected void onStop() {
		Log.d(TAG, "**** got onStop");

		super.onStop();
		nativeOnActivityStopped(this);
	}

	// Implemented in C++.

	private static native void nativeInitGPGS(AppActivity act);

	public static native void nativeOnActivityResult(Activity activity, int requestCode, int resultCode, Intent data);

	public static native void nativeOnActivityCreated(Activity activity, Bundle savedInstanceState);

	private static native void nativeOnActivityDestroyed(Activity activity);

	private static native void nativeOnActivityPaused(Activity activity);

	private static native void nativeOnActivityResumed(Activity activity);

	private static native void nativeOnActivitySaveInstanceState(Activity activity, Bundle outState);

	private static native void nativeOnActivityStarted(Activity activity);

	private static native void nativeOnActivityStopped(Activity activity);
}
