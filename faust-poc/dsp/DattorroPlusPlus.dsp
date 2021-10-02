import("stdfaust.lib");

declare dattorro_rev_2 author "Jakob Zerbian, Liam Wall";
declare dattorro_rev_2 license "MIT-style STK-4.3 license";

dattorro_rev_2(pre_delay, bw, i_diff1, i_diff2, decay, d_diff1, d_diff2, damping) =
    si.bus(2) : + : *(0.5) : predelay : bw_filter : diffusion_network : reverb_loop
with {
    // allpass using delay with fixed size
    allpass_f(t, a) = (+ <: @(t),*(a)) ~ *(-a) : mem,_ : +;
    // allpass using delay with fixed size, and two extra taps
    allpass_f_taps(t, a, tap_1, tap_2) = (+ <: @(t),*(a),@(tap_1),@(tap_2)) ~ *(-a) : mem,_,_,_ : +,_,_;
    // allpass using delay with excursion
    allpass_exc(t, a, i) = (+ <: de.fdelayltv(2, t+200, t + 16*os.oscsin(0.8 + i/2.87983274932749287429847)),*(a)) ~ *(-a) : mem,_ : +;

    // input pre-delay and diffusion
    predelay = @(pre_delay);
    bw_filter = *(bw) : +~(mem : *(1-bw));
    diffusion_network = allpass_f(142, i_diff1) : allpass_f(107, i_diff1) : allpass_f(379, i_diff2) : allpass_f(277, i_diff2);
    damp = (*(1-damping) : +~*(damping) : *(decay)), _,_;

    tapL(m, c, l, r) = c, l + m*c, r;
    tapR(m, c, l, r) = c, l, m*c + r;


    // /********* left  output,  all  wet  *********/
    // [x] accumulator =  0.6  X  node48_54[266]
    // [x] accumulator +=  0.6  x  node48_54[2974]
    // [x] accumulator -=  0.6  X  node55_59[1913]
    // [x] accumulator +=  0.6  X  node59_63[1996]
    // [x] accumulator -=  0.6  X  node24_30[1990]
    // [x] accumulator -=  0.6  x  node31_33[187]
    // [x] YL  =  accumulator -  0.6  X  node33_39[1066]

    // /********* right  output,  all  wet  *********/
    // [x] accumulator =  0.6  X  node24_30[353]
    // [x] accumulator +=  0.6  X  node24_30[3627]
    // [x] accumulator -=  0.6  X  node31_33[1228]
    // [x] accumulator +=  0.6  X  node33_39[2673]
    // [x] accumulator -=  0.6  X  node48_54[2111]
    // [x] accumulator -=  0.6  X  node55_59[335]
    // [x] YR  =  accumulator -  0.6  X  node59_63[121]

    // Contains node23_24
    decay_diffusion_1a = allpass_exc(672,-d_diff1, 0),_,_;

    // node24_30
    z_4453 = (_ <: @(4453), @(1990), @(353), @(3627) : _,_,+ : _,*(-0.6),*(0.6)),_,_ : _,_,ro.cross(2),_ : _,+,+;

    // Contains node31_33
    decay_diffusion_2a = (allpass_f_taps(1800, d_diff2, 187, 1228) : _,*(-0.6),*(-0.6)),_,_ : _,_,ro.cross(2),_ : _,+,+;

    // node33_39
    z_3720 = (_ <: @(3720), @(1066), @(2673) : _,*(-0.6),*(0.6)),_,_ : _,_,ro.cross(2),_ : _,+,+;

    // Contains node46_48
    decay_diffusion_1b = allpass_exc(908,-d_diff1, 0),_,_;

    // node48_54
    z_4217 = (_ <: @(4217), @(266), @(2974), @(2111) : _,+,_ : _,*(0.6),*(-0.6)),_,_ : _,_,ro.cross(2),_ : _,+,+;

    // Contains node55_59
    decay_diffusion_2b = (allpass_f_taps(2656, d_diff2, 1913, 335): _,*(-0.6),*(-0.6)),_,_ : _,_,ro.cross(2),_ : _,+,+;

    // node59_63
    z_3163 = (_ <: @(3163), @(1996), @(121) : _,*(0.6),*(-0.6)),_,_ : _,_,ro.cross(2),_ : _,+,+ ;

    // Three channels: (1) is the output to loop, (2) is Left, and (3) is right.
    reverb_chain(loop_in, in)
         = (loop_in+in)
        <: (_,_,_)
         : decay_diffusion_1a
         : z_4453
         : damp
         : decay_diffusion_2a
         : z_3720
         : (*(decay),_,_)
         : (+(in),_,_)
         : decay_diffusion_1b
         : z_4217
         : damp
         : decay_diffusion_2b
         : z_3163
         : (*(decay),_,_);


    reverb_loop = reverb_chain~_ : (si.block(1),_,_);
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