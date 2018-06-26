open Belt;

let plant: Plant.plant = {
  location: {
    zip: Plant.ZipDe("40239"),
    country: Plant.DE,
  },
  threshold: Plant.Celsius(15.0),
};

let isSafe = (plant: Plant.plant, tomorrow: Api.forecast) =>
  switch (plant.threshold) {
  | Celsius(x) =>
    if (tomorrow.main.temp_min -. 272.15 < x) {
      Some(
        "Hol sie rein es werden "
        ++ Js.Float.toString(tomorrow.main.temp_min -. 272.15),
      );
    } else {
      None;
    }
  | Kelvin(x) =>
    if (tomorrow.main.temp_min < x) {
      Some(
        "Hol sie rein es werden " ++ Js.Float.toString(tomorrow.main.temp_min),
      );
    } else {
      None;
    }
  };

let command = forecasts =>
  Option.getWithDefault(
    Option.flatMap(Api.tomorrowNight(forecasts), isSafe(plant)),
    "Kann draussen bleiben",
  );

Api.forecast(plant)
|> Js.Promise.then_(forecasts => Js.Promise.resolve(command(forecasts)))
|> Js.Promise.then_(cmd => Js.Promise.resolve(Js.log(cmd)));
