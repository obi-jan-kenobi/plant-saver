let plant: Plant.plant = {
  location: {
    zip: Plant.ZipDe("40239"),
    country: Plant.DE,
  },
  threshold: Plant.Celsius(15.0),
};

Api.forecast(plant)
|> Js.Promise.then_(forecasts =>
     Js.Promise.resolve(Api.tomorrow(forecasts))
   )
|> Js.Promise.then_((tomorrow: option(Api.main)) =>
     switch (tomorrow) {
     | Some(main) =>
       switch (plant.threshold) {
       | Celsius(x) =>
         if (main.main.temp_min -. 272.15 < x) {
           Js.log(
             "Hol sie rein es werden "
             ++ Js.Float.toString(main.main.temp_min -. 272.15),
           );
         } else {
           Js.log("alles fein");
         };
         Js.Promise.resolve("");
       | Kelvin(x) =>
         if (main.main.temp_min < x) {
           Js.log(
             "Hol sie rein es werden "
             ++ Js.Float.toString(main.main.temp_min),
           );
         } else {
           Js.log("alles fein");
         };
         Js.Promise.resolve("");
       }

     | None => Js.Promise.resolve("")
     }
   );
