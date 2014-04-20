<?php

class FinUrls
{
    private $_allGoodSym = '*';
    private $_goodSym = '+';
    private $_okSym = '-';
    private $_badSym = 'o';
    private $_data_host = "http://chartapi.finance.yahoo.com";
    //    private $_data_range = "/instrument/1.0/{0}/chartdata;type={1};range={2}/csv/";
    //    private $_data_yearly = "/instrument/1.0/{0}/chartdata;type={1};ys={2};yz={3}/csv/";
    private $_mdata;

    public function __construct()
    {
        $this->_mdata = array();
    }

    public function allGoodSym() { return $this->_allGoodSym; }
    public function goodSym() { return $this->_goodSym; }
    public function okSym() { return $this->_okSym; }
    public function badSym() { return $this->_badSym; }

    public function mdata() { print_r($this->_mdata); }

    /**
     * Where range can be 1d,5d,1m,3m,6m,ytd,1y,2y,5y,10y
     **/
    public function dataRange($symbol, $type, $range, $period=null)
    {
        return $this->_data_host . "/instrument/1.0/" . $symbol . "/chartdata;type=" .
        $type . ";range=" . $range . "/csv/" . ($period ? "?period=" . $period : '');
    }

    public function dataYearly($symbol, $type, $start, $ylen)
    {
        return $this->_data_host . "/instrument/1.0/" . $symbol . "/chartdata;type=" . $type .
        ";ys=" . $start . ";yz=" . $ylen . "/csv/" .  ($period ? "?period=" . $period : '');
    }

//     public function readData($symbol, $type, $range, $period=null)
//     {
//         $url = $this->dataRange($symbol, $type, $range, $period);
//         echo $url . '<br>';
//         $data = file_get_contents($url);
//         $lines = explode('
// ', $data);
        
//         echo '<pre>';
//         foreach ($lines as $l)
//         {
//             // skip over meta-data
//             if (strpos($l, ':'))
//             {
//                 //                continue;
//             }
//             echo $l . '
// ';
//         }
//         echo '</pre>';
//     }

    public function storeData($symbol, $type, $range, $period=null)
    {
        $url = $this->dataRange($symbol, $type, $range, $period);
        error_log('url: ' . $url);
        $data = file_get_contents($url);
        $lines = explode('
', $data);
        
        foreach ($lines as $l)
        {
            // skip over meta-data
            if (strpos($l, 'values:') !== false)
            {
                $values = explode(',', substr($l, strlen('values:')));
            }
            else if (strpos($l, ':') !== false)
            {
                continue;
            }
            else
            {       
                $csv = explode(',', $l);
                $d = array();
                for ($i = 1; $i < count($csv); $i++)
                {
//                     echo $values[$i] . $period . ' = ' . $csv[$i] . '<br>';
                    //                 $ar = array($values[$i] . $period => $csv[$i]);
                    //$this->_mdata['\''.$csv[0].'\'']['\''.$values[$i] . $period.'\''] = $csv[$i];
                    $this->_mdata[$csv[0]][$values[$i] . $period] = $csv[$i];
                }
            }
        }
    }

    public function dateColumns()
    {
        $dates = '';
        foreach($this->_mdata as $kdate => $data)
        {
            $dates .= '<td>' . $kdate . '</td>';
        }
        return $dates;
    }

    public function checkData()
    {
        $res = '';
        foreach($this->_mdata as $kdate => $data)
        {
            $d = $data;
            if (!array_key_exists('ema10', $d) 
                || !array_key_exists('ema20', $d) 
                || !array_key_exists('close', $d) 
                || !array_key_exists('upper', $d) 
                || !array_key_exists('lower', $d)
                || !array_key_exists('macd', $d)
                || !array_key_exists('signal', $d))
                continue;
            $mid = $d['upper'] - ($d['upper'] - $d['lower'])/2;

            if ($d['ema10'] > $d['ema20']
                && $d['close'] > $d['ema10']
                && $d['close'] > $mid
                && $d['macd'] > $d['signal']
                && $d['macd'] > 0) {
                $res .= ' ' . $this->_allGoodSym;
            } else if ($d['ema10'] > $d['ema20']) {
                $res .= ' ' . $this->_goodSym;
            } else if ($d['macd'] > $d['signal']
                     && $d['close'] > $mid) {
                $res .= ' ' . $this->_okSym;
            } else {
                $res .= ' ' . $this->_badSym;
            }
        }
        // returns the string with the pass/no-pass days
        return $res;
    }

    public function numLastSuccessDays($data)
    {
        $n = 0;
        $pos = count($data)-1;
        while ($data[$pos] === $this->_allGoodSym)
        {
            $n++;
            $pos--;
        }
        return $n;
    }

}

?>