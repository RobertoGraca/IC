#ifndef GOLOMB_ENCODER_H
#define GOLOMB_ENCODER_H

#include "BitStream2.h"
#include "math.h"


enum GolombType
{
	GOLOMB, 
	GOLOMB_RICE, 
	EXP_GOLOMB   
};

class GolombEncoder
{
public:

	GolombEncoder(uint64_t m_,GolombType golombType = GolombType::GOLOMB);

	
	//Se a cache não for suficientemente longa para conter L bits, o byte é preenchido e os bits restantes são armazenados em cache no BitBuffer.
	//Os bits restantes são armazenados em cache no BitBuffer, e depois são devolvidos falsos, indicando que a cache é insuficiente para conter os elementos de código codificados.  
	
	bool encode(uint64_t num);

	/*
		Terminar o processo de codificação. Passa os dados do buffer de bits para o buffer de bytes, depois preencha 64bits com zeros e determina
		se o totalCodeLength é um múltiplo de 8, se não, continuar a preencher com 0's.
	*/
	bool close();

	/*
		Quando o buffer estiver cheio, processar os dados completos do buffer (escrever em ficheiro, enviar...) , reinicializar o tampão e reutilizá-lo.

	*/
	void resetBuffer();

	/*
		Colocação de amortecedores do exterior
	*/
	void setBuffer(uint8_t *buffer, uint64_t len);

	// Retorna o comprimento dos bytes codificados, que deve ser um múltiplo de 8
	uint64_t getToalCodeLength()
	{
		return bitStream.getTotalCodeLength();
	}

private:
	uint64_t rice_golombEncode(uint64_t num);
	uint64_t exp_golombEncode(uint64_t num);

private:

	BitOutputStream bitStream;

	GolombType type; // Tipo de código Golomb
	uint64_t m;
	uint64_t k;
};


GolombEncoder::GolombEncoder(uint64_t para, GolombType golombType)
{
	type = golombType;

	switch (golombType)
	{
	case GOLOMB:
		m = para;
		k = log(m) / log(2);
		break;
	case GOLOMB_RICE:
		break;
	case EXP_GOLOMB:
		k = para;
		m = -1;
		break;
	default:
		k = -1;
		m = -1;
		break;
	}

}

void GolombEncoder::setBuffer(uint8_t *buffer, uint64_t len)
{
	bitStream.setBuffer(buffer, len);
}

void GolombEncoder::resetBuffer()
{
	bitStream.resetBuffer();
}

bool GolombEncoder::encode(uint64_t num)
{
	uint64_t len = 0;
	switch (type)
	{
	case GOLOMB:
		break;

	case GOLOMB_RICE:
		len = rice_golombEncode(num);
		break;

	case EXP_GOLOMB:
		len = exp_golombEncode(num);
		break;
	default:
		break;
	}

	return bitStream.freeLength() >= len; // Verdadeiro se houver espaço suficiente para armazenar os elementos de código codificados; caso contrário, falso
}

bool GolombEncoder::close()
{
	return bitStream.flush();
}


///////////////////////////////////////////////////////////////////////////////////////////
//
// Implementação de diferentes códigos Golomb 
//
//////////////////////////////////////////////////////////////////////////////////////////

// Golomb Rice
uint64_t GolombEncoder::rice_golombEncode(uint64_t num)
{
	uint64_t q = num >> k;
	uint64_t r = num & (m - 1);

	auto len = q + 1 + k; // Comprimento dos elementos codificados após a codificação

	/*
	 Não determina se a cache está cheia, apenas a coloca dentro e a coloca em cache, se não estiver cheia
	*/
	bitStream.putBit(1, q);
	bitStream.putBit(0);

	for (int i = 0; i < k; i++)
	{
		bitStream.putBit(static_cast<bool>(r & 0x01));
		r >>= 1;
	}

	return len; // Devolve o comprimento do elemento de código codificado
}

// Exp Golomb
uint64_t GolombEncoder::exp_golombEncode(uint64_t num)
{
	if (k != 0)
		num = num + pow(2, k) - 1;

	// 0ª ordem código exp-golom
	auto m = static_cast<int>(log(num + 1) / log(2)); // prefixo
	auto info = static_cast<int>(num + 1 - pow(2, m)); // compensação

	auto len = 2 * m + 1; // Comprimento dos elementos codificados após a codificação

	// Escrever m um 0
	bitStream.putBit(0, m);
	// Escrever m um 1
	bitStream.putBit(1);

	// Escrever informação m-1 bits
	for (int i = 0; i < m ; i++)
	{
		bitStream.putBit(static_cast<bool>(info & 0x01));
		info >>= 1;
	}

	return len;
}

#endif