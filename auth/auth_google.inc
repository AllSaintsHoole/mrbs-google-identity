<?php
namespace MRBS;

require_once __DIR__ . '/../vendor/autoload.php';

/*****************************************************************************
*
*   File name       auth_google.inc
*
*   Description     Authenticate users with Google Connect OpenID.
*
*   Notes           To use this authentication scheme, download and install the
*         Google APIs Client Library for PHP (through composer - recommended)
*
*         composer require google/apiclient:^2.0
*
*         Config:
*         - $google_client_id (required)
*         - $google_domain    (optional)
*         
*         DB Schema:
*         - id (auto)
*         - name (full name, e.g. 'Bob Jones')
*         - level (MRBS access level)
*         - password_hash (Google Account ID)
*         - email (email address at last login)
*
******************************************************************************/



/* authValidateUser($user, $pass)
 * 
 * Checks if the id token is valid (and in the correct domain $google_domain, if applicable)
 * 
 * $user  - The user's Google account ID - verified through the auth id token
 * $pass  - The user's auth ID Token which needs verifying, & from which information can be gathered
 * 
 * Returns:
 *   0        - The pair are invalid or do not exist
 *   non-zero - The pair are valid
 */

if (!isset($tbl_users))
{
  $tbl_users = $db_tbl_prefix.'users';
}


$user_list_link = 'users.php';


function authValidateUser($user, $pass)
{
  global $google_domain,
         $google_client_id;

  // Get $id_token via $user.
  $id_token = $pass;

  $client = new \Google_Client(['client_id' => $CLIENT_ID]);
  $payload = $client->verifyIdToken($id_token);

  if ($payload) {
    $userid = $payload['sub'];

    // Check client ID not faked
    if ($google_client_id != $payload['aud']) {
      return 0; // Invalid client id
    }

    // If request specified a G Suite domain:
    if (isset($google_domain)) {
      if ($google_domain != $payload['hd']) {
        return 0; // Invalid domain
      }
    }
    
  } else {
    return 0; // Invalid ID token
  }
  updateUserInDb($payload);
  setSessionFromUser($payload);
  return 1; // User validated
}

/* authGetUserLevel($user)
 * 
 * Determines the user's access level
 * 
 * $user - The user name (not used for DB query)
 *
 * Returns:
 *   The users access level
 */
function authGetUserLevel($user)
{
  global $tbl_users;
  // User not logged in, user level '0'
  if(!isset($user))
  {
    return 0;
  }
  
  $result = db()->query1("SELECT level FROM $tbl_users WHERE password_hash=? LIMIT 1",
                       array(utf8_strtolower($_SESSION['userDetails']['sub'])));

  return ($result == -1) ? 0 : $result;
}


function updateUserInDb($userDetails)
{
  global $tbl_users;

  // does user already exist
  $result = db()->query1("SELECT count(*) FROM $tbl_users WHERE password_hash=? LIMIT 1",
                       [utf8_strtolower($userDetails['sub'])]);
  if ($result > 0) {
    db()->command("UPDATE $tbl_users SET name = ?, email = ? WHERE password_hash = ?",
                [ $userDetails['name'], $userDetails['email'], $userDetails['sub'] ]);
  } else {
    $level = 1;

    // make admin if there are no other users in the db
    $result = db()->query1("SELECT count(*) FROM $tbl_users");
    if ( ! $result > 0)
    {
      $level = 2;
    }

    db()->command("INSERT INTO $tbl_users (level, name, password_hash, email) VALUES (?, ?, ?, ?)",
                  [ $level, $userDetails['name'], $userDetails['sub'], $userDetails['email'] ]);
  }
}