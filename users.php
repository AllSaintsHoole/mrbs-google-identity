<?php

namespace MRBS;

require "defaultincludes.inc";

$page_level['users.php'] = 2; // Admin only

// Check the user is authorised for this page
checkAuthorised();


// if GET
if (isset($_GET['user']) && isset($_GET['level'])) {
  $admins = db()->query1("SELECT count(*) FROM $tbl_users WHERE level=?", [2]);

  // don't allow admin to lock themselves out
  if ($admins > 1 || $_GET['level'] == 2)
  {
    $user = filter_var($_GET['user'], FILTER_SANITIZE_NUMBER_INT);
    $level = filter_var($_GET['level'], FILTER_SANITIZE_NUMBER_INT);
    db()->command("UPDATE $tbl_users SET level = ? WHERE id = ?",
                [ $level,  $user ]);
  }
  header('Location: users.php');
}


$roles = [
  0 => 'locked out',
  1 => 'user',
  2 => 'admin'
];

$users = db()->query("SELECT id, name, level, email FROM $tbl_users ORDER BY level DESC, name");



// PRESENTATION

print_header($day, $month, $year, isset($area) ? $area : "", isset($room) ? $room : "");
?>
<h3>Showing all users who have previously logged in</h3>
<p>You can change user roles to give them admin capabilities, or lock them out of the system completely.</p>


<table class="admin_table display dataTable no-footer" role="grid">
  <thead>
    <tr role="row" style="height: 26px;">
      <th>Name</th>
      <th>Rights</th>
      <th>Email address</th>
      <th>Change rights to:</th>
    </tr>
  </thead>
  <tbody>
    <?php for ($i = 0; ($user = $users->row_keyed($i)); $i++): ?>
    <tr role="row">
      <td>
        <a title="<?php echo $user['name']; ?>" href="report.php?creatormatch=<?php echo urlencode($user['name']) ?>"><?php echo $user['name']; ?></a>
      </td>
      <td>
        <span title="<?php echo $user['level'] ?>"></span>
        <div class="string"><?php echo $roles[$user['level']] ?></div>
      </td>
      <td>
        <div class="string">
          <a href="mailto:<?php echo $user['email']; ?>" target="_blank"><?php echo $user['email']; ?></a>
        </div>
      </td>
      <td>
        <div class="string">
          <?php foreach ($roles as $key => $role): ?>
          <?php if ($user['level'] != $key): ?>
          <a class="user_roles" href="?user=<?php echo $user['id'] ?>&level=<?php echo $key ?>"><?php echo $role ?></a>
          <?php endif ?>
          <?php endforeach ?>
        </div>
      </td>
    </tr>
    <?php endfor ?>
  </tbody>
</table>
<?php print_theme_footer() ?>