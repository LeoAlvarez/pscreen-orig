<?php

class screenerHelper
{
    private $outFile;

    public function __construct($f)
    {
        $this->outFile = $f;
    }        

    public function symbolData($fu, $sym, $range)
    {
        $fu->storeData($sym, 'close', $range);
        $fu->storeData($sym, 'ema', $range, 10);
        $fu->storeData($sym, 'ema', $range, 20);
        $fu->storeData($sym, 'bollinger', $range);
        $fu->storeData($sym, 'macd', $range);
    }

    public function readStocks($fname)
    {
        $stockfile = str_replace(array('"', ' '), '', file_get_contents($fname));
        $symbols = explode('
', trim($stockfile));
        return $symbols;
    }


    public function yfcLink($sym)
    {
        $url = 'http://finance.yahoo.com/echarts?s=' . $sym . '+Interactive#symbol=aapl;range=6m;compare=;indicator=ema%2810,20)+bollinger+volume+macd;charttype=ohlc;crosshair=on;ohlcvalues=0;logscale=off;source=';
        return '<a href="' . $url . '" target="_ychart"><img src="yahoo_favicon.png"></a>';
    }

    public function stockchartsLink($sym)
    {
        $url = 'http://stockcharts.com/h-sc/ui?nIndicators=3&nOverlays=3&nMaxOverlays=3&nMaxIndicators=3&predefChanged=false&isPermanentId=false&setPermanentId=false&defaultChart=false&chartId=&style=&dataTotalDays=0&inputsymbol=' .
        $sym .
        '&originalsymbol=' .
        $sym . '&inputperiod=D&originalperiod=D&currentSetting=0|||&curCustomSetting=0|||&customSettingsCount=0&curChartList=&curChartName=&curChartNameIndex=0&symbol=' .
        $sym .
        '&period=D&period2=D&dataRange=fill&bar=5&gap=0&bars-fill=&years=0&months=6&days=0&bars-predef=&start=&end=&symStyle=type%3D%22candle%22&renkoUnit=atr&renkoBoxSize=14&renkoPriceField=close&kagiUnit=atr&kagiReversal=14&kagiPriceField=hilo&chartSize=Landscape&customWidth=Landscape&customHeight=530&chartSkin=default&showVolume=overlay&dataLog=true&dataSignalP=sig&dataSignalV=sig&layerTitles=true&antiAliasOn=true&axisLabels=true&overType_=B&overArgs_=&overType_0=EMA&overArgs_0=20&overType_1=EMA&overArgs_1=10&overType_2=BB&overArgs_2=20,2&indType_=B&indArgs_=&indLoc_=below&indType_0=MACD&indArgs_0=12,26,9&indLoc_0=below&indType_1=CCI&indArgs_1=20&indLoc_1=below&syncHeight=';
        return '<a href="' . $url . '" target="_schart">' . $sym . '</a>';
    }

    public function writeOutput($str)
    {
        echo $str;
        file_put_contents($this->outFile, $str, FILE_APPEND | LOCK_EX);
    }

}

?>