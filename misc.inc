<?php

function myecho($str, $from, $type) {
    echo $from . '[' . $type . ']=> ' . $str;
}
function visitorIP()
{ 
    if(isset($_SERVER['HTTP_X_FORWARDED_FOR']))
    {
        $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
    }
    else
    {
        $ip = $_SERVER['REMOTE_ADDR'];
    }
 
    return trim($ip);
}

function storeIP()
{
    $ip = date('d M, Y H:i') . ': ' . visitorIP() . '
';
    error_log('storeIP');
    file_put_contents('screenerIPs', $ip, FILE_APPEND | LOCK_EX);
}

function tableHead($title)
{
    $html = <<<HTML
<section class=tholder>
<table class="pure-table pure-table-striped">
  <thead>
    <tr>
       <th>#</th>
       <th></th>
       <th></th>
       <th colspan=4>$title</th>
    </tr>
  </thead>
HTML;
    return $html;
}

function goodDay($trend)
{
    $col = 'hashG';
    if ($trend == 0)
        $col = 'hashN';
    else if ($trend == -1)
        $col = 'hashR';
            
    return '<div class="gline"><div class="vline hash ' . $col . '"></div><div class="vline hash hashW"></div></div>';
}

function badDay($trend)
{
    $col = 'hashG';
    if ($trend == 0)
        $col = 'hashN';
    else if ($trend == -1)
        $col = 'hashR';
            
    return '<div class="gline"><div class="vline hash hashW"></div><div class="vline hash ' . $col . '"></div></div>';
}

?>