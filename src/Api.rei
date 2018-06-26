type temp = {
  temp: float,
  temp_max: float,
  temp_min: float,
};

type forecast = {
  main: temp,
  dt: int,
  dt_txt: string,
};

type forecasts = {list: array(forecast)};

let forecast: Plant.plant => Js.Promise.t(forecasts);

let tomorrowNight: forecasts => option(forecast);
