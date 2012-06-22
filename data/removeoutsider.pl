#!/usr/bin/perl -w

while(<>){
   if(/contour/){
      @f = split;
      $good = 1;
      for($i = 2; $i <= $#f; $i++){
         if( ($i % 2 == 0) && ($f[$i] > 450)){
            $good = 0;
            last;
         }
      }
      if($good == 1){ print }
   }
   else{
      print
   }
}
