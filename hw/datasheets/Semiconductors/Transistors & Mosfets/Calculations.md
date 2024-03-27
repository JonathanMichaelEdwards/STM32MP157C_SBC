Mosfet Calculations
-------------

1. Find **V_GS** limit
    - V_GS = V_G - V_S - Look in Datasheet to determine states 
2. Find **R_DS** min
    - P = R_DS * I_D^2 - Power of Mosfet
3. Find P_Dissipation - limit of mosfet without heat sink
   - P_Diss = (max(T_J) - T_A) / R_THJA

&nbsp;

Transistor  Calculations
-------------

* Check Relationship between DC Currents and Gains in the resources folder
1. beta_dc = I_C / I_B
2. Select sutiable I_C & dc gain

