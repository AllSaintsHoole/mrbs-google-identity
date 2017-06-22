<?php // -*-mode: PHP; coding:utf-8;-*-
namespace MRBS;
// $Id: footer.inc 1781 2011-01-31 11:02:00Z cimorrison $

function print_theme_footer()
{
?>
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
  </body>
</html>
<?php

}
 