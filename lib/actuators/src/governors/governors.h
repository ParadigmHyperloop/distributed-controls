class Governor {
public:
  virtual ~Governor(){}
  virtual bool checkValue(float val);
};
