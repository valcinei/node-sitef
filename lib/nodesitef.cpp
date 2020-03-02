#include "nodesitef.hpp"
#include "promises/promises.hpp"

void *handler;

Value carregarDLL(const CallbackInfo &info)
{
  Env env = info.Env();

  if (handler)
    return Boolean::New(env, true);

  if (info.Length() < 1)
    napi_throw_error(env, "0", "Informe o caminho da DLL.");
  else if (!info[0].IsString())
    napi_throw_type_error(env, "1", "O caminho informado não é uma string válida.");
  else
  {
    string path = info[0].ToString().Utf8Value();
    handler = dlopen(path.c_str(), RTLD_LAZY);

    if (!handler)
      napi_throw_type_error(env, "2", "Não foi possível carregar a DLL.");
    else
      return Boolean::New(env, true);
  }

  return env.Null();
}

int configuraIntSiTefInterativo(const char *ip, const char *terminal, const char *loja, const char *reservado)
{
  if (!handler)

    throw("Carregue a DLL do SiTef!");

  ConfiguraIntSiTefInterativo configuraSitef = (ConfiguraIntSiTefInterativo)dlsym(handler, "ConfiguraIntSiTefInterativo");

  return configuraSitef(
      ip,
      terminal,
      loja,
      reservado);
}

int verificaPresencaPinPad()
{
  if (!handler)
    throw("Carregue a DLL do SiTef!");

  VerificaPresencaPinPad verificaPresenca = (VerificaPresencaPinPad)dlsym(handler, "VerificaPresencaPinPad");

  return verificaPresenca();
}

int escreveMensagemPermanentePinPad(const char *mensagem)
{
  if (!handler)
    throw("Carregue a DLL do SiTef!");

  EscreveMensagemPermanentePinPad escreveMensagem = (EscreveMensagemPermanentePinPad)dlsym(handler, "EscreveMensagemPermanentePinPad");

  return escreveMensagem(mensagem);
}

int leSimNaoPinPad(const char *mensagem)
{
  if (!handler)
    throw("Carregue a DLL do SiTef!");

  LeSimNaoPinPad escreveMensagem = (LeSimNaoPinPad)dlsym(handler, "LeSimNaoPinPad");

  return escreveMensagem(mensagem);
}

int iniciaFuncaoSiTefInterativo(int funcao, const char *valor, const char *cupomFiscal, const char *dataFiscal, const char *horaFiscal, const char *operador, const char *paramAdicionais)
{
  if (!handler)
    throw("Carregue a DLL do SiTef!");

  IniciaFuncaoSiTefInterativo iniciaFuncao = (IniciaFuncaoSiTefInterativo)dlsym(handler, "IniciaFuncaoSiTefInterativo");

  return iniciaFuncao(
      funcao,
      valor,
      cupomFiscal,
      dataFiscal,
      horaFiscal,
      operador,
      paramAdicionais);
}

int continuaFuncaoSiTefInterativo(int *comando, long *tipoCampo, int *tamMinimo, int *tamMaximo, char *buffer, int tamBuffer, int continua)
{
  if (!handler)
    throw("Carregue a DLL do SiTef!");

  ContinuaFuncaoSiTefInterativo continuaFuncao = (ContinuaFuncaoSiTefInterativo)dlsym(handler, "ContinuaFuncaoSiTefInterativo");

  return continuaFuncao(
      comando,
      tipoCampo,
      tamMinimo,
      tamMaximo,
      buffer,
      tamBuffer,
      continua);
}

void finalizaFuncaoSiTefInterativo(int confirma, const char *cupomFiscal, const char *dataFiscal, const char *horaFiscal, const char *paramAdicionais)
{
  if (!handler)

    throw("Carregue a DLL do SiTef!");

  FinalizaFuncaoSiTefInterativo finalizaFuncao = (FinalizaFuncaoSiTefInterativo)dlsym(handler, "FinalizaFuncaoSiTefInterativo");
  finalizaFuncao(confirma, cupomFiscal, dataFiscal, horaFiscal, paramAdicionais);
}

Object Init(Env env, Object exports)
{
  exports.Set(
      String::New(env, "carregarDLL"),
      Function::New(env, carregarDLL));

  exports.Set(
      String::New(env, "configuraIntSiTefInterativo"),
      Function::New(env, ConfigPromise::Create));

  exports.Set(
      String::New(env, "verificaPresencaPinPad"),
      Function::New(env, VerificarPresencaPromise::Create));

  exports.Set(
      String::New(env, "leSimNaoPinPad"),
      Function::New(env, LeSimNaoPromise::Create));

  exports.Set(
      String::New(env, "escreveMensagemPermanentePinPad"),
      Function::New(env, EscreverMensagemPromise::Create));

  exports.Set(
      String::New(env, "iniciaFuncaoSiTefInterativo"),
      Function::New(env, IniciaFuncaoPromise::Create));

  exports.Set(
      String::New(env, "continuaFuncaoSiTefInterativo"),
      Function::New(env, ContinuaFuncaoPromise::Create));

  exports.Set(
      String::New(env, "finalizaFuncaoSiTefInterativo"),
      Function::New(env, FinalizaFuncaoPromise::Create));

  return exports;
}

NODE_API_MODULE(nodesitef, Init);