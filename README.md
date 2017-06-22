# MRBS - Auth through Google Identity (OpenID)

An extension to [MRBS](mrbs.sourceforge.net) to add Google Identity as an Auth option.

Repo also includes a theme inspired by the [AdminLTE](https://github.com/almasaeed2010/AdminLTE) design and colours.


## Usage

1. Install MRBS (tested with 1.6.1)

2. Edit `functions.inc` to include the extra lines of code below into the function `is_private_event($privacy_status)` (demonstrated in [functions.inc](functions.inc))

```php
// ADDITION BY CHRISTOPHER WILKINSON FOR ALL SAINTS CHURCH (2016)
global $private_override_by_auth;
if(checkAuthorised(TRUE) && isset($private_override_by_auth) && $private_override_by_auth ) // if user is logged in && overide requested
{
  $privacy_status = FALSE;
}
// END OF BY CHRISTOPHER WILKINSON FOR ALL SAINTS CHURCH (2016)
```

3. Add the following files to MRBS:

    * auth/auth_google.inc
    * session/session.google.inc
    * users.php

4. Delete the file `edit_users.php` from the MRBS installation.

5. Ensure your theme has the necessary header and footer meta tags & javascript for Google Identity. Alternatively, use the `allsaints` theme provided (updating `$theme = "allsaints";` in your `config.inc.php`).

Header:

```php
<script src="https://apis.google.com/js/platform.js" async defer></script>
<meta name="google-signin-scope" content="profile email">
<?php if (isset($google_domain)): ?>
<meta name="google-signin-hosted_domain" content="<?php echo $google_domain ?>">
<?php endif ?>
<?php if (isset($google_client_id)): ?>
<meta name="google-signin-client_id" content="<?php echo $google_client_id ?>">
<?php endif ?>
```

Footer:

```html
<script>
function onSignIn(googleUser) {
  var profile = googleUser.getBasicProfile();
  // The ID token you need to pass to your backend:
  var id_token = googleUser.getAuthResponse().id_token;
  var payload = {
    Action: 'SetName',
    NewUserName: profile.getId(),
    NewUserPassword: id_token,
    TargetURL: window.location.pathname + window.location.search
  };
  console.log(payload);
  var form = document.createElement('form');
  form.style.visibility = 'hidden'; // no user interaction is necessary
  form.method = 'POST'; // forms by default use GET query strings
  form.action = 'admin.php';
  for (key in payload) {
    var input = document.createElement('input');
    input.name = key;
    input.setAttribute('value',payload[key]);
    form.appendChild(input); // add key/value pair to form
  }
  document.body.appendChild(form); // forms cannot be submitted outside of body
  form.submit(); // send the payload and navigate
}
</script>
```

6. Install dependencies through composer

```
composer install
```

(or install [Google API PHP Client](https://github.com/google/google-api-php-client/) into manually, updating the `require` in [auth_google.inc](auth/auth_google.inc) if necessary).

7. Rename `config.inc.php-default` to `config.inc.php` and configure your settings.

8. Sit back, relax, and have a cup of tea! (hopefully, unless you need to do some debugging! ;) )

---

## Known issues

* The theme `allsaints` does not work without a database connection, or when applying MRBS updates. You'll need to temporarily change the theme back to `default` while upgrading the database.