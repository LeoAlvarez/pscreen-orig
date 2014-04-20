<?php

  /**
   * For shits and grins
   **/

function getRequest($index, $val = -1)
{
    if (isset($_REQUEST[$index]) && $_REQUEST[$index])
    {
        $req = $_REQUEST[$index];
        return $req;
    }
    return $val; 
}  

function showGains($amt, $rate, $num)
{
  $tot = $amt;
  echo '
<div class=gains>
<h3>Rate: ' . $rate . '<br>starting amt: ' . $amt . '</h3>
<table >
  <tr class=htr><th>num</th><th>gain</th><th>tot</th></tr>
';
  $class = 'a';
  for ($i = 1; $i <= $num; $i++)
    {
      $gain = $tot * $rate;
      $tot += $gain;
      echo '  <tr class=' . $class . '><th>' . $i . '</th><td>' . number_format($gain, 2) .
	'</td><td>' . number_format($tot,2) . '</td></tr>
';
      $class = $class == 'a' ? 'b' : 'a';
    }
  echo '</table>
</div>

';
}

function showDoubles($amt, $num)
{
  $tot = $amt;
  echo '
<div class=gains>
<h3>Starting amt: ' . $amt . '</h3>
<table >
  <tr class=htr><th>num</th><th>gain</th></tr>
';
  $class = 'a';
  for ($i = 1; $i <= $num; $i++)
    {
      echo '  <tr class=' . $class . '><th>' . $i . '</th><td>' . number_format($tot, 2) .
	'</td></tr>
';
      $tot *= 2;
      $class = $class == 'a' ? 'b' : 'a';
    }
  echo '</table>
</div>

';
}

echo '<style>
h3 {font-size: 85%; }
.gains { float: left; margin-right: 4px; border: 1px solid #aae; background: #eee; }
.gains table { padding-left: 4px; }
table { font-size: 75%; }
tr.htr { background: #b33; color: white; }
tr.a { background: #eee; }
tr.b { background: #ddd; }
.gains table td { text-align: right; }
.gains table th, .gains table td { margin-right: 1px; } 
</style>
';

$startAmt = getRequest('s', 20000);
//showDoubles(20000, 20);
//echo '<p>';
$px = 50;
showGains($startAmt, .02, $px);
echo '<p>';
showGains($startAmt, .03, $px);
echo '<p>';
showGains($startAmt, .05, $px);
echo '<p>';
showGains($startAmt, .10, $px);
echo '<p>';
showGains($startAmt, .15, $px);
echo '<p>';
showGains($startAmt, .2, $px);
echo '<p>';
showGains($startAmt, .25, $px);
echo '<p>';
showGains($startAmt, .3, $px);

?>