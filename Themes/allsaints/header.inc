<?php
namespace MRBS;

// $Id: header.inc 2784 2013-11-21 10:48:22Z cimorrison $


// Print the page header
function print_theme_header($day, $month, $year, $area, $room, $simple=false)
{
  global $search_str, $locale_warning;

  // Need to set the timezone before we can use date()
  if ($simple)
  {
    // We don't really care what timezone is being used
    mrbs_default_timezone_set();
  }
  else
  {
    // This will set the correct timezone for the area
    get_area_settings($area);  
  }

  // If we dont know the right date then make it up
  if (!$day)
  {
    $day   = date("d");
  }
  if (!$month)
  {
    $month = date("m");
  }
  if (!$year)
  {
    $year  = date("Y");
  }

  // PRESENTATION

  http_headers();
  echo DOCTYPE;
?>

<html>

  <?php print_head($simple) ?>

  <?php
  // Put the filename in as a class to aid styling.
  // (Use a class rather than id to avoid specificity problems)
  ?>
  <body class="non_js  <?php echo htmlspecialchars(this_page('.php')) ?>">

    <?php
    // Add a class of "js" so that we know if we're using JavaScript or not
    // and remove the non_js class (it's sometimes useful to know that we're
    // not running JavaScript)
    ?>
    <script type="text/javascript">
      //<![CDATA[
      $('body').addClass('js').removeClass('non_js');
      //]]>
    </script>

    <div class="screenonly">

<?php // show a warning if this is using a low version of php
  if (!empty($locale_warning))
  {
    echo "[Warning: ".$locale_warning."]";
  }
?>

  <header class="main-header" id="banner">

  <?php print_header_site_info($day, $month, $year, $search_str) ?>

  </header><!-- /.banner -->
</div>

<div id="contents">
<?php

} // end of print_theme_header()







function print_head($simple=false)
{
    global $mrbs_company,
         $mrbs_company_more_info,
         $google_domain,
         $google_client_id;
  ?>
  <head>
    <meta charset=" <? echo get_charset() ?>">
    <title>
      <?php if($mrbs_company_more_info) {
        echo $mrbs_company_more_info . " | " . $mrbs_company;
      }
      else {
        echo get_vocab("mrbs") . " | " . $mrbs_company;
      } ?>
    </title>
    
    <?php require_once "style.inc" ?>
    
    <?php
    if (!$simple)
    {
      require_once "js.inc";
    }
    ?>

    <link rel="stylesheet" href="Themes/allsaints/custom.css" type="text/css">
    <script src="https://apis.google.com/js/platform.js" async defer></script>
    <meta name="google-signin-scope" content="profile email">
    <?php if (isset($google_domain)): ?>
    <meta name="google-signin-hosted_domain" content="<?php echo $google_domain ?>">
    <?php endif ?>
    <?php if (isset($google_client_id)): ?>
    <meta name="google-signin-client_id" content="<?php echo $google_client_id ?>">
    <?php endif ?>
  </head>
  <?php
} // end of print_head()




// Print the basic site information.   This function is used for all headers, including
// the simple header, and so mustn't require any database access.
function print_header_site_info($day, $month, $year, $search_str)
{
  global $mrbs_company,
         $mrbs_company_url,
         $mrbs_company_logo,
         $mrbs_company_more_info,
         $mrbs_company_short;

  $user = getUserName();

  if (!empty($mrbs_company_url)) {
    echo "<a class=\"logo\" id=\"company\" href=\"$mrbs_company_url\">\n";
        }
  else {
    echo "<a class=\"logo\" id=\"company\" href=\"index.php\">\n";
  }?>
      <?php
      echo "<span id=\"logo\">\n";
      if (empty($mrbs_company_logo))
      {
        echo "<strong>$mrbs_company_short</strong>\n";
      }
      else
      {
        // Suppress error messages in case the logo is a URL and allow_url_fopen
        // is not enabled in php.ini
        $logo_size = @getimagesize($mrbs_company_logo);
        echo "<img src=\"$mrbs_company_logo\" " . $logo_size[3] . " alt=\"$mrbs_company\">\n";
      }
      echo "</span>\n";
      if (!empty($mrbs_company_more_info))
      {
        echo "<span id=\"more-info\">$mrbs_company_more_info</span>\n";
      }
      else {
        echo '<a href="index.php">' . get_vocab("mrbs") . '</a>';
      }
      ?>
  </a><!-- /.item -->
  <div class="item">
    <form action="day.php" method="get" id="Form1">
      <div>
        <?php
        // Give the form id as the optional fifth parameter because we want
        // the form to be automatically submitted when the datepicker is closed
        genDateSelector("", $day, $month, $year, "Form1");
        if (!empty($area))
        {
          echo "<input type=\"hidden\" name=\"area\" value=\"$area\">\n";
        }
        if (!empty($room))
        {
          echo "<input type=\"hidden\" name=\"room\" value=\"$room\">\n";
        }
        // Although the datepicker will automatically go to the new date when
        // the date is changed, we still need a submit button because there
        // are occasions when you want to go to the date without changing it -
        // for example when you've been on a Search or Report page
        echo "<input type=\"submit\" value=\"" . get_vocab("goto") . "\">\n";
        ?>
        </div>
    </form>
  </div><!-- /.item -->
    <?php
    // Provide a link to the list of bookings awaiting approval
    // (if there are any enabled areas where we require bookings to be approved)

    $approval_somewhere = some_area('approval_enabled', TRUE);
    if ($approval_somewhere && (authGetUserLevel($user) >= 1))
    {
      $n_outstanding = get_entries_n_outstanding($user);
      echo "<a class=\"item" .
            (($n_outstanding > 0) ? " hilghlight" : '') .
            "\" href=\"pending.php?day=$day&amp;month=$month&amp;year=$year&amp;area=$area" .
            ((!empty($room)) ? "&amp;room=$room" : "") .
            "\">\n";
      echo "<span>$n_outstanding ". get_vocab("outstanding") . "</span>";
      echo "</a>\n"; // -- /.item --
    }
    ?>
  <?php
  $query_str = "day=$day&amp;month=$month&amp;year=$year";
  if (!empty($area))
  {
    $query_str .= "&amp;area=$area";
  }
  if (!empty($room))
  {
    $query_str .= "&amp;room=$room";
  }

  echo "<a class='item' href=\"index.php\">\n";
  echo "<span>" . get_vocab("gototoday") . "</span>\n";
  echo "</a>\n";

  echo "<a class='item' href=\"help.php?$query_str\">\n";
  echo "<span>" . get_vocab("help") . "</span>\n";
  echo "</a>\n";

  echo "<a class='item' href=\"admin.php?$query_str\">\n";
  echo "<span>" . get_vocab("rooms") . "</span>\n";
  echo "</a>\n";

  echo "<a class='item' href=\"report.php?$query_str\">\n";
  echo "<span>" . get_vocab("report") . "</span>\n";
  echo "</a>\n";
  ?>

  <div class='item'>
    <form id="header_search" method="get" action="search.php">
      <div>
        <a href="search.php?advanced=1"><?php echo get_vocab("search") ?>:</a>
        <input type="search" name="search_str" value="<?php echo htmlspecialchars($search_str) ?>" required>
        <input type="hidden" name="day"        value="<?php echo $day        ?>">
        <input type="hidden" name="month"      value="<?php echo $month      ?>">
        <input type="hidden" name="year"       value="<?php echo $year       ?>">
        <?php
        if (!empty($area))
        {
          echo "<input type=\"hidden\" name=\"area\" value=\"$area\">\n";
        }
        if (!empty($room))
        {
          echo "<input type=\"hidden\" name=\"room\" value=\"$room\">\n";
        }
        ?>
      </div>
    </form>
  </div><!-- /.item -->
  <?php
  // For session protocols that define their own logon box...
  if (function_exists('MRBS\PrintLogonBox'))
  {
    echo "<div class='item' id=\"logon_box\">\n";
    PrintLogonBox();
    echo "</div>\n";
  }
}
