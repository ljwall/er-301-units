declare er301_in1 "In";
declare er301_out1 "Out";

// take 3-4% cpu for 20 of these without -vec -vs 4
// take 3-4% cpu for 20 of these with -vec -vs 4
//process = @(2000)*0.5;

// 40 of these take ~19% CPU without -vec -vs 4
// 40 of these take ~19% CPU with -vec -vs 4
//process = _ <: @(2000), @(1000), @(100), _ :> (/(4));

process = _ <: @(2000),_ :> _;
