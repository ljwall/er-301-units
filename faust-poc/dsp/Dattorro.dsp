import("stdfaust.lib");

// Dattorro reverb with difusion parameters as per https://ccrma.stanford.edu/~dattorro/EffectDesignPart1.pdf
reverb(band_width, decay, damping) = re.dattorro_rev(0, band_width, 0.75, 0.625, decay, 0.7, 0.5, damping);

// Contols (will become parameters in the er-301 object)
band_width_ctrl = hslider("BandWidth", 0.6, 0, 1, 0.001) : si.smoo;
decay_ctrl = hslider("Decay", 0.8, 0, 1, 0.001) : si.smoo;
damping_ctr = hslider("Damping", 0.25, 0, 1, 0.001) : si.smoo;
dry_wet_ctr = hslider("DryWet", 0.25, 0, 1, 0.001) : si.smoo;

dry_wet_mix(mix, dry_l, dry_r, wet_l, wet_r) = (1-mix) * dry_l, (1-mix) * dry_r, mix * wet_l, mix * wet_r :> _,_;

// Name the ins and outs of the `process` function for use in the er-301 object
declare er301_in1 "InL";
declare er301_in2 "InR";
declare er301_out1 "OutL";
declare er301_out2 "OutR";

process = _,_ <: _,_,reverb(band_width_ctrl, decay_ctrl, damping_ctr): dry_wet_mix(dry_wet_ctr);
