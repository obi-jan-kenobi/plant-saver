type zip =
  | ZipDe(string);

type country =
  | DE;

type temperature =
  | Kelvin(float)
  | Celsius(float);

type location = {
  zip,
  country,
};

let countryToString = country =>
  switch (country) {
  | DE => "de"
  };

let zipToString = zip =>
  switch (zip) {
  | ZipDe(x) => x
  };

type plant = {
  location,
  threshold: temperature,
};
