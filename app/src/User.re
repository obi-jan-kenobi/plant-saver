type id =
  | UserId(string);
type email =
  | Email(string);
type password =
  | Password(string);

type user = {
  id,
  email,
  password,
};
