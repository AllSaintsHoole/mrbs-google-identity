<?php
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// AMMEND THIS FILE TO INCLUDE THE FOLLOWING CODE
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// line 2781 (as of MRBS 1.6.1)

// returns true if event should be considered private based on
// config settings and event's privacy status (passed to function)
function is_private_event($privacy_status) 
{
  global $private_override;
  if ($private_override == "private" )
  {
    $privacy_status = TRUE;
  }
  elseif ($private_override == "public" )
  {
    $privacy_status = FALSE;
  }

  // ADDITION BY CHRISTOPHER WILKINSON FOR ALL SAINTS CHURCH (2016)
  global $private_override_by_auth;
  if(strlen(getUserName()) > 0 && isset($private_override_by_auth) && $private_override_by_auth ) // if user is logged in && overide requested
  {
    $privacy_status = FALSE;
  }
  // END OF BY CHRISTOPHER WILKINSON FOR ALL SAINTS CHURCH (2016)

  return $privacy_status;
}
