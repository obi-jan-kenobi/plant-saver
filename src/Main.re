open Belt;

[@bs.deriving abstract]
type msg = {
  [@bs.as "to"] to_: string,
  from: string,
  subject: string,
  text: string,
  html: string,
};

[@bs.module "@sendgrid/mail"] external setApiKey : string => unit = "";
[@bs.module "@sendgrid/mail"] external send : msg => Js.Promise.t(unit) = "";

let to_ = Option.getExn(Js.Dict.get(Node.Process.process##env, "EMAIL_TO"));
let from = Option.getExn(Js.Dict.get(Node.Process.process##env, "EMAIL_FROM"));

let user: User.user = {
  id: User.UserId("1"),
  email: User.Email(to_),
  password: User.Password("1")
};

let plant: Plant.plant = {
  id: Plant.PlantId("Habanero"),
  owner: user.id,
  location: {
    zip: Plant.ZipDe("40239"),
    country: Plant.DE,
  },
  threshold: Plant.Celsius(15.0),
};

let needsSaving = (plant: Plant.plant, tomorrow: Api.forecast) =>
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
    Option.flatMap(Api.tomorrowNight(forecasts), needsSaving(plant)),
    "Kann draussen bleiben",
  );

let appid = Option.getExn(Js.Dict.get(Node.Process.process##env, "APPID"));
let apikey =
  Option.getExn(Js.Dict.get(Node.Process.process##env, "SENDGRID_API_KEY"));

setApiKey(apikey);

Api.forecast(appid, plant)
|> Js.Promise.then_(forecasts => Js.Promise.resolve(command(forecasts)))
|> Js.Promise.then_(cmd => {
  let msg = msg(~to_=to_, ~from=from, ~subject="Plant Status", ~text=cmd, ~html=cmd);
  Js.Promise.resolve(send(msg));
});
