import("stdfaust.lib");

declare andromeda author "Liam Wall";
declare andromeda license "MIT-style STK-4.3 license";

andromeda(decay, low_pass, high_pass) = _,_ : + : *(0.5) : (+~chain) <: chain_l, chain_r with {
  line = de.fdelayltv(2, 28800);
  taps = (0.047, 0.120, 0.134, 0.146, 0.158, 0.169, 0.180, 0.190, 0.200, 0.209, 0.217, 0.233, 0.240, 0.244, 0.225, 0.247);
  //epsilon = 0.00013; // Around 1Hz
  epsilon1 =  0.000130;
  epsilon2 =  0.000063;
  epsilon3 =  0.000043;
  epsilon4 =  0.000020;
  mods = x,xq,-x,-xq , y,yq,-y,-yq, z,zq,-z,-zq, a,aq,-a,-aq letrec {
    'xq = os.impulse + xq - epsilon1*x;
    'x = epsilon1 * (xq - epsilon1 *x) + x;

    'yq = os.impulse + yq - epsilon2*y;
    'y = epsilon2 * (yq - epsilon2 *y) + y;

    'zq = os.impulse + zq - epsilon3*z;
    'z = epsilon3 * (zq - epsilon3 *z) + z;

    'aq = os.impulse + aq - epsilon4*a;
    'a = epsilon4 * (aq - epsilon4 *a) + a;
  };
  limiter(x) = 2 * x / sqrt(x*x +4);
  depth = ba.sec2samp(0.004);
  chain = _
       <: par(i, ba.count(taps), line(ba.sec2samp(ba.take(i+1, taps)) + depth*ba.take(1 + (i % (ba.count(mods))), mods)))
       :> /(ba.count(taps))
        : fi.lowpass(1, low_pass)
        : fi.highpass(1, high_pass)
        : *(decay)
        : limiter;

  line_out = de.delay(24000);
  taps_l = (0.060, 0.137, 0.175, 0.190);
  taps_r = (0.077, 0.112, 0.160, 0.212);
  chain_l = _ <: par(i, ba.count(taps_l), line_out(ba.take(i+1, taps_l) : ba.sec2samp)) :> /(ba.count(taps_l));
  chain_r = _ <: par(i, ba.count(taps_r), line_out(ba.take(i+1, taps_r) : ba.sec2samp)) :> /(ba.count(taps_r));
};

declare er301_in1 "InL";
declare er301_in2 "InR";
declare er301_out1 "OutL";
declare er301_out2 "OutR";

process = _,_ <: _,_,andromeda(decay_ctrl, low_ctrl, high_ctrl): dry_wet_mix(dry_wet_ctr) with {
  decay_ctrl = hslider("Decay", 0.8, 0, 5, 0.001) : si.smoo;
  low_ctrl = hslider("HighCut", 20000, 100, 20000, 100) : min(20000);
  high_ctrl = hslider("LowCut", 20, 20, 20000, 100) : max(20);
  dry_wet_ctr = hslider("DryWet", 0.25, 0, 1, 0.001) : si.smoo;
  dry_wet_mix(mix, dry_l, dry_r, wet_l, wet_r) = (1-mix) * dry_l, (1-mix) * dry_r, mix * wet_l, mix * wet_r :> _,_;
};
