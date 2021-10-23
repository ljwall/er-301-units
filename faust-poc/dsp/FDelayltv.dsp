import("stdfaust.lib");

// Name the ins and outs of the `process` function for use in the er-301 object
declare er301_in1 "Length";
declare er301_in2 "In";
declare er301_out1 "Out";

// 19% CPU for 10 of these with -vec -vs 4
// 16% CPU for 10 of these without -vec -vs 4
process = de.fdelayltv(2, 1000);
