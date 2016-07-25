#include "Key.hpp"

#include "utility/ScopedTimer.hpp"
#include "security/SecurityConstants.hpp"


int KeyPrivate::mKCT=0;

KeyPrivate::KeyPrivate( QVariantMap map, bool isPublic )
	: mKey(isPublic?"":map.contains("privateKey")?map["privateKey"].toString():"")
	, mPubKey(map.contains("publicKey")?map["publicKey"].toString():"")
	, mKID(mKCT++)
{
	OC_METHODGATE();
	//qDebug()<<"KeyPrivate::KeyPrivate( QVariantMap key, bool isPublic ):"<<map<<isPublic <<" for " <<mKID<<"/"<<mKCT;
	parse(isPublic);
}

KeyPrivate::KeyPrivate( QString key, bool isPublic )
	: mKey(isPublic?"":key)
	, mPubKey(isPublic?key:"")
	, mKID(mKCT++)
{
	OC_METHODGATE();
//	qDebug()<<"KeyPrivate::KeyPrivate( QString key, bool isPublic ):"<<key<<isPublic <<" for " <<mKID<<"/"<<mKCT;
	parse(isPublic);
}


KeyPrivate::KeyPrivate(quint32 bits)
	: mKID(mKCT++)
{
	OC_METHODGATE();
//	qDebug()<<"KeyPrivate::KeyPrivate(quint32 bits):"<<bits<<" for " <<mKID<<"/"<<mKCT;
	ScopedTimer timer("Key Generation");
	mPKI.generateKeyPair(bits);
	if(mPKI.isValid()){
		mKey=mPKI.getPEMKey();
		mPubKey=mPKI.getPEMPubkey();
		mID=hash(mPubKey);
	}
	else{
		qWarning()<<"ERROR: keywas not valid after generation";
	}
}


void KeyPrivate::parse(bool isPublic)
{
	ScopedTimer timer("Key Parsing");
	if(isPublic){
		QByteArray raw=mPubKey.toUtf8();
		if(0!=mPKI.parsePublicKey(raw)){
			qWarning()<<"ERROR: Could not parse pubkey: "<<raw;
		}
	}
	else{
		QByteArray raw=mKey.toUtf8();
		if(0!=mPKI.parseKey(raw)){
			qWarning()<<"ERROR: Could not parse key: "<<raw;
		}
		mPubKey=mPKI.getPEMPubkey();
		if(mPubKey.isEmpty()){
			qWarning()<<"ERROR: Could not get pubkey from key: "<<raw;
		}
	}
	mID=hash(mPubKey);
}

KeyPrivate::KeyPrivate()
	: mKID(mKCT++)
{
	OC_METHODGATE();
//	qDebug()<<"KeyPrivate::KeyPrivate()" <<" for " <<mKID<<"/"<<mKCT;
}

KeyPrivate::~KeyPrivate(){
	OC_METHODGATE();
//	qDebug()<<"KeyPrivate::~KeyPrivate()" <<" for " <<mKID<<"/"<<mKCT;
}


QString KeyPrivate::hash(QString input)
{
	OC_FUNCTIONGATE();
	QCryptographicHash ch(OCTOMY_KEY_HASH);
	ch.addData(input.toUtf8());
	return ch.result().toHex().toUpper();
}


//////////////////////////////////////////////////////////////////////



Key::Key(const Key &other)
	: d_ptr(new KeyPrivate(
				(!other.d_func()->mKey.isEmpty())
				?
					(other.d_func()->mKey)
				  :
					(other.d_func()->mPubKey),
				other.d_func()->mKey.isEmpty()
				)
			)
{
	OC_METHODGATE();
//	qDebug()<<"Key::Key(constKey &other)"<<other.d_func()->mKey<<other.d_func()->mPubKey;
}




Key::Key( QVariantMap map, bool isPublic )
	: d_ptr(new KeyPrivate(map, isPublic))
{
	OC_METHODGATE();
//	qDebug()<<"Key::Key( QVariantMap map, bool isPublic )"<< map<<isPublic;
}


Key::Key( QString key, bool isPublic )
	: d_ptr(new KeyPrivate(key, isPublic))
{
	OC_METHODGATE();
//	qDebug()<<"Key::Key( QString key, bool isPublic):"<<key<<isPublic;
}



Key::Key( quint32 bits )
	: d_ptr(new KeyPrivate(bits))
{
	OC_METHODGATE();
//	qDebug()<<"Key::Key( quint32 bits ):"<<bits;
}


Key::Key(KeyPrivate &dd)
	: d_ptr(&dd)
{
	OC_METHODGATE();
//	qDebug()<<"Key::Key(KeyPrivate &dd)";
}


Key::Key(Key && other) : Key() {
	OC_METHODGATE();
	swap(*this, other);
//	qDebug()<<"Key::Key(Key && other) : Key()";
}

Key & Key::operator=(Key other) {
	OC_METHODGATE();
	swap(*this, other);
//	qDebug()<<"Key & Key::operator=(Key other)";
	return *this;
}

bool Key::operator==(Key &other){
	OC_METHODGATE();
//	qDebug()<<"bool Key::operator==(Key &other)";
	return ( (other.d_func() == this->d_func() ) || (other.key() == key() ) );
}


bool Key::operator!=(Key &other){
	OC_METHODGATE();
//	qDebug()<<"bool Key::operator!=(Key &other)";
	return !operator==(other);
}

void swap(Key& first, Key& second) /* nothrow */ {
	OC_FUNCTIONGATE();
	using std::swap;
//	qDebug()<<"void swap(Key& first, Key& second) /* nothrow */";
	swap(first.d_ptr, second.d_ptr);
}

