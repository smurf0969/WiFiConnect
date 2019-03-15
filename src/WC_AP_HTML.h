/*!
 * @file WC_AP_HTML.h
 *
 * HTML snippits to build the Access Point portal and the Parameters Portal.
 *
 * Written by Stuart Blair
 *
 * GNU General Public License v3.0 licence, all text here must be included in any redistribution and you should receive a copy of the license file.
 *
 */
#ifndef WC_AP_HTML
#define WC_AP_HTML ///< Define to stop re-inclusion
/*! \def char AP_HTTP_HEAD[] PROGMEM
Start of HTML output
 */
const char AP_HTTP_HEAD[] PROGMEM            = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";
/*! \def AP_HTTP_STYLE[] PROGMEM 
Style for our access point 
*/
const char AP_HTTP_STYLE[] PROGMEM           = "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>";
/** Scripts for our page */
const char AP_HTTP_SCRIPT[] PROGMEM          = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
/** End of the header section and beginning of the body */
const char AP_HTTP_HEAD_END[] PROGMEM        = "</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>";
/** Start of our HTMl configuration Form */
const char AP_HTTP_PORTAL_OPTIONS[] PROGMEM  = "<form action=\"/wifi\" method=\"get\"><button>Configure WiFi</button></form><br/><form action=\"/0wifi\" method=\"get\"><button>Configure WiFi (No Scan)</button></form><br/><form action=\"/i\" method=\"get\"><button>Info</button></form><br/><form action=\"/r\" method=\"post\"><button>Reset</button></form>";
/** HTML snippet for wifi scanning */
const char AP_HTTP_ITEM[] PROGMEM            = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
/** HTML form for saving wifi connection details */
const char AP_HTTP_FORM_START[] PROGMEM      ="<form method=\"get\" action=\"wifisave\"><label>SSID</label><input id=\"s\" name=\"s\" length=32 placeholder=\"SSID\"><label>Password</label><input id=\"p\" name=\"p\" length=64 type=\"password\" placeholder=\"password\"><br/>";;
/** HTML snippet for our custom parameters */
const char AP_HTTP_FORM_PARAM[] PROGMEM      = "<br/><input id='{i}' name='{n}' maxlength={l} placeholder='{p}' value='{v}' {c}>";
/** The end of our HTML Form */
const char AP_HTTP_FORM_END[] PROGMEM        = "<br/><button type='submit'>save</button></form>";
/** HTML snippet to recan for networks */
const char AP_HTTP_SCAN_LINK[] PROGMEM       = "<br/><div class=\"c\"><a href=\"/wifi\">Scan</a></div>";
/** HTML snippet for saved confirmation */
const char AP_HTTP_SAVED[] PROGMEM           = "<div>Credentials Saved<br />Trying to connect ESP to network.<br />If it fails reconnect to AP to try again</div>";
/** End of the HTML page */
const char AP_HTTP_END[] PROGMEM             = "</div></body></html>";
/** HTML snippet for our custom parameters portal form */
const char AP_HTTP_PORTAL_PARAM_OPTIONS[] PROGMEM  = "<form action=\"/params\" method=\"get\"><button>Configure Parameters</button></form><br/><form action=\"/i\" method=\"get\"><button>Info</button></form><br/><form action=\"/r\" method=\"post\"><button>Reset</button></form>";
/** HTML snippet for our custom parameters save */
const char AP_HTTP_FORM_PARAM_START[] PROGMEM      ="<form method=\"get\" action=\"wifisave\">";
#endif
