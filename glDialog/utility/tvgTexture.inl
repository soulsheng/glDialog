//----------------------------------------------------------------
inline void Texture::useBind()
{
	assert( mTexState != TEXTURE_STATE_UNDEFINED );

	if ( this->mTexState == TEXTURE_STATE_EMPTY )
	{
		glBindTexture( GL_TEXTURE_2D , DEFAULT_EMPTY_TEXTURE_ID );
	}
	else
	{
		glBindTexture( GL_TEXTURE_2D , mTextureID );
	}
}
//----------------------------------------------------------------
inline bool Texture::isEmptyTexture() const
{
	assert( isInitialised() );

	if ( this->mTexState == TEXTURE_STATE_EMPTY )
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------
inline bool Texture::isBinded() const
{
	if ( this->mTexState == TEXTURE_STATE_LOADED 
		|| this->mTexState == TEXTURE_STATE_UNLOADED )
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------
inline bool Texture::isInitialised() const
{
	if ( this->mTexState == TEXTURE_STATE_UNDEFINED )
	{
		return false;
	}
	return true;
}