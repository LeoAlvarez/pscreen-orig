<?php
include './header.inc';

require './misc.inc';
require './FinUrls.class.inc';
require './screenerHelper.inc';


ini_set("max_execution_time", 10000);

$symCacheFile = "./symdata_" . date('d_M_Y');
$processingFinishedFile = "./symdata_" . date('d_M_Y') . '._done';
$sh = new screenerHelper($symCacheFile);

storeIP();

// has processing finished?
if (file_exists($processingFinishedFile)) {
    error_log('DONE file exists');
    echo file_get_contents($symCacheFile);
    exit;
}

if (file_exists($symCacheFile)) {
    unlink($symCacheFile);
 }

$html = '
<span class="date">' . date('d M, Y') . ':<span> 
price > 10 DEMA > 20 DEMA, price > mid bollinger, MACD crossover</h2>
        </div>
      </div>
    </div>
  </div>
</div>
<div id="mytable">... please wait ... <img src="http://www.loadingicons.com/images/icons/spinning_wheel_throbber_02-25-2007_06-08-2007_06-12-2007.gif"></div>
';
$sh->writeOutput($html);

$html = '
<script>
YUI().use("datatable", function (Y) {

';

$syms = $sh->readStocks('symbols.csv');
$finurl = new FinUrls();
$range = '1m';

$i = 0;
$iend = 700;
$res = '';
$tstart = time();


// 2 html sections, one for trending, one for the maybes
//$html = tableHead('Trending Now');
//echo $html;
//$sh->writeOutput($html);
//$html = '';

//$html2 = tableHead('Maybe Turning');
//$fullMsg .= $html;
$nsuccess = 0;
$nsuccess2 = 0;

$html .= '  var rowdata = [
';

$firstseen = false;
foreach ($syms as $sym) {
    error_log($sym . '-> ' . date('h:i:s A'));
    set_time_limit(100); // will this work?
    $finurl->clean();
    $sh->symbolData($finurl, $sym, $range);
    // replace the o & t's with the column mockup
    $symdata = $finurl->checkData();
    $dta = explode(' ', $symdata);
    $ndta = count($dta);

    // success for one is not success for the other!
    $success = ($dta[$ndta-1] == $finurl->allGoodSym() ? true : false);
    $success2 = ($dta[$ndta-1] == $finurl->okSym() ? true : false);
    $lastGoodDays = $finurl->numLastSuccessDays($dta);
    error_log('symdata: ' . $sym . ' ' . $symdata . ' [ ' . $lastGoodDays . ' ] ' . ' => tot: ' . $ndta);
//     $finurl->mdata();
        
    // replace with table cells
    $data = str_replace(array($finurl->badSym(), $finurl->okSym(),
                              $finurl->goodSym(), $finurl->allGoodSym()),
                        array(badDay(-1), badDay(0), goodDay(0), goodDay(1)),
                        $symdata);

    $allGoodDays = substr_count($symdata, $finurl->allGoodSym());
    // only show if it's a candidate
    if ($success || $success2) {
        // keep track of each type of success
        if ($success) $nn = $nsuccess++;
        if ($success2) $nn = $nsuccess2++;
        $goodStyle = '';
        if ($allGoodDays > 0) {
            $ratio = number_format($lastGoodDays / $allGoodDays, 2);
        }
        if ($ratio == 1) { 
            $goodStyle = 'class=good' . $allGoodDays;
        }
        if ($success || $success2) {
            $row = ($firstseen ? ',' : '') . '  
    { id: ' . $nsuccess . ',
      sclink: \'' . $sh->stockchartsLink($sym) . '\',
      ylink: \'' . $sh->yfcLink($sym) . '\',
      hashdata: \'' . $data . '\',
      numgooddays: ' . $lastGoodDays . ',
      totgooddays: '. $allGoodDays . ', 
      ratio: ' . $ratio . '
    }';
            $firstseen = true;
        }
        /// ***** LEO RESUME HERE
    }
    if ($success) {
        $rowdata .= $row;
    }
    if ($success2) {
        $rowdata2 .= $row;
    }
}

$html .= $rowdata . '
  ],
  table = new Y.DataTable({
     columns: [
        { key: "id", label: "", sortable: true}, 
        { key: "sclink", label: " ", allowHTML: true, formatter: "{value}" },
        { key: "ylink", label: " ", allowHTML: true, formatter: "{value}" },
        { key: "hashdata", label: "trending up ", allowHTML: true, formatter: "{value}"},
        { key: "numgooddays", label: "consec ", sortable: true},
        { key: "totgooddays", label: "tot ", sortable: true},
        { key: "ratio", label: " ", sortable: true}
     ],
     data: rowdata
   }), 
   tbldiv = Y.one("#mytable");

   tbldiv.setHTML(" ");
   table.render("#mytable");
});
</script>
';

$html .= '

</body>
</html>
';
//echo $html;
$sh->writeOutput($html);
file_put_contents($processingFinishedFile, 'done', FILE_APPEND | LOCK_EX);

?>
