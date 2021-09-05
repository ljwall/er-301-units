import("stdfaust.lib");

declare dattorro_rev_2 author "Jakob Zerbian, Liam Wall";
declare dattorro_rev_2 license "MIT-style STK-4.3 license";


dattorro_rev_2(pre_delay, bw, i_diff1, i_diff2, decay, d_diff1, d_diff2, damping) =
    si.bus(2) : + : *(0.5) : predelay : bw_filter : diffusion_network <: ((si.bus(4) :> _,_) ~ (reverb_network : ro.cross(2)))
with {
    // allpass using delay with fixed size
    allpass_f(t, a) = (+ <: @(t),*(a)) ~ *(-a) : mem,_ : +;
    // allpass using delay with excursion
    allpass_exc(t, a, i) = (+ <: de.fdelaylti(1, t+200, t + 16*os.oscsin(0.8 + i/2.87983274932749287429847)),*(a)) ~ *(-a) : mem,_ : +;

    // input pre-delay and diffusion
    predelay = @(pre_delay);
    bw_filter = *(bw) : +~(mem : *(1-bw));
    diffusion_network = allpass_f(142, i_diff1) : allpass_f(107, i_diff1) : allpass_f(379, i_diff2) : allpass_f(277, i_diff2);

    // reverb loop
    reverb_network = par(i, 2, block(i)) with {
        d = (672, 908, 4453, 4217, 1800, 2656, 3720, 3163);
        block(i) = allpass_exc(ba.take(i+1, d),-d_diff1, i) : @(ba.take(i+3, d)) : damp :
            allpass_f(ba.take(i+5, d), d_diff2) : @(ba.take(i+5, d)) : *(decay)
        with {
            damp = *(1-damping) : +~*(damping) : *(decay);
        };
    };
};

// Dattorro reverb with difusion parameters as per https://ccrma.stanford.edu/~dattorro/EffectDesignPart1.pdf
reverb(band_width, decay, damping) = dattorro_rev_2(0, band_width, 0.75, 0.625, decay, 0.7, 0.5, damping);

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
