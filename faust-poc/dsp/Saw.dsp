import("stdfaust.lib");

declare er301_input "In";
declare er301_output "Out";

// process = os.sawtooth;
process = _ <: re.dattorro_rev_default :> _;
