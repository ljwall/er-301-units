import("stdfaust.lib");

// Name the ins and outs of the `process` function for use in the er-301 object
declare er301_in1 "Length";
declare er301_in2 "In";
declare er301_out1 "Out";

process = de.sdelay(1000, 8);