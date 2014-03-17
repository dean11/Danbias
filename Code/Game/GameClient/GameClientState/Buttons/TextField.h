/********************************************************************
 * Text field that allows multiple lines.
 *
 * Written by Dan Andersson, 2014
 ********************************************************************/

#ifndef DANBIAS_CLIENT_TEXT_FIELD_H
#define DANBIAS_CLIENT_TEXT_FIELD_H

#include <string>
#include <vector>
#include "ButtonRectangle.h"
#include "OysterMath.h"
#include "Utilities.h"

namespace DanBias { namespace Client
{
	template<typename Owner>
	class TextField : public ButtonRectangle<Owner>
	{
	public:
		TextField();
		TextField( ::std::wstring backgroundTexture, ::Oyster::Math::Float4 textColor, ::Oyster::Math::Float4 backColor, Owner owner, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_Height );
		TextField( ::std::wstring backgroundTexture, ::Oyster::Math::Float4 textColor, ::Oyster::Math::Float4 backColor, ::Oyster::Math::Float4 hovCol, Owner owner, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_Height );
		virtual ~TextField();

		virtual void RenderText();

		const ::std::wstring & operator[]( unsigned int i ) const;
			  ::std::wstring & operator[]( unsigned int i );

		void SetFontHeight( ::Oyster::Math::Float h );
		void SetLineSpacing( ::Oyster::Math::Float ls );

		void SetFontColor(::Oyster::Math::Float4 col) { this->textColor = col; } ;
		void SetBackColor( Oyster::Math::Float4 c ) { this->backColor = c; };
		Oyster::Math::Float4 GetBackColor( ) { return this->backColor; };

		void SetBottomAligned();
		void SetTopAligned();

		unsigned int GetNumLines() const;
		unsigned int GetMaxLineLength() const;

		void ReserveLines( unsigned int num );
		void ClearText();
		void AppendText( const ::std::wstring &text );

		void PopBack();
		void PopFront();

	private:
		bool isBottomAligned;
		::Oyster::Math::Float fontHeight, lineSpacing;
		::std::vector<::std::wstring> lines;
	};

// IMPLEMENTATIONS //////////////////////////////////////////////////

	template<typename Owner>
	TextField<Owner>::TextField()
		: ButtonRectangle()
	{
		this->fontHeight = 0.025f;
		this->lineSpacing = 0.001f;
		this->isBottomAligned = true;
	}

	template<typename Owner>
	TextField<Owner>::TextField( ::std::wstring backgroundTexture, ::Oyster::Math::Float4 textColor, ::Oyster::Math::Float4 backColor, Owner owner, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize )
		: ButtonRectangle( backgroundTexture, L"", textColor, backColor, backColor, backColor, owner, pos, size, resize )
	{
		this->fontHeight = 0.025f;
		this->lineSpacing = 0.001f;
		this->isBottomAligned = true;
	}

	template<typename Owner>
	TextField<Owner>::TextField( ::std::wstring backgroundTexture, ::Oyster::Math::Float4 textColor, ::Oyster::Math::Float4 backColor, ::Oyster::Math::Float4 hovCol, Owner owner, Oyster::Math::Float3 pos, Oyster::Math::Float2 size, ResizeAspectRatio resize = ResizeAspectRatio_Height )
		: ButtonRectangle( backgroundTexture, L"", textColor, backColor, hovCol, backColor, owner, pos, size, resize )
	{
		this->fontHeight = 0.025f;
		this->lineSpacing = 0.001f;
		this->isBottomAligned = true;
	}

	template<typename Owner>
	TextField<Owner>::~TextField() {}

	template<typename Owner>
	void TextField<Owner>::RenderText()
	{
		::Oyster::Math::Float lineStep = this->fontHeight + this->lineSpacing;
		::Oyster::Math::Float2 rowSize = ::Oyster::Math::Float2( this->size.x, this->fontHeight );
		::Oyster::Math::Float3 fieldTopLeft = this->pos - Float3( this->size.x * 0.5f, this->size.y * 0.25f, 0.0001f );
		::Oyster::Math::Float3 topLeft = fieldTopLeft;

		if( this->isBottomAligned )
		{
			topLeft.y += this->size.y - lineStep;

			auto line = this->lines.rbegin();
			for( ; line != this->lines.rend(); ++line )
			{
				if( topLeft.y < fieldTopLeft.y )
					break;

				::Oyster::Graphics::API::RenderText( (*line), topLeft, rowSize, this->fontHeight, this->textColor );
				topLeft.y -= lineStep;				
			}
		}
		else
		{
			topLeft.y += this->lineSpacing;

			auto line = this->lines.begin();
			for( ; line != this->lines.end(); ++line )
			{
				if( topLeft.y >= fieldTopLeft.y + this->size.y )
					break;

				::Oyster::Graphics::API::RenderText( (*line), topLeft, rowSize, this->fontHeight, this->textColor );
				topLeft.y += lineStep;				
			}
		}
	}

	template<typename Owner>
	const ::std::wstring & TextField<Owner>::operator[]( unsigned int i ) const
	{
		return this->lines[(::std::vector<::std::wstring>::size_type)i];
	}

	template<typename Owner>
	::std::wstring & TextField<Owner>::operator[]( unsigned int i )
	{
		return this->lines[(::std::vector<::std::wstring>::size_type)i];
	}

	template<typename Owner>
	void TextField<Owner>::SetFontHeight( ::Oyster::Math::Float h )
	{
		this->fontHeight = h;
	}

	template<typename Owner>
	void TextField<Owner>::SetLineSpacing( ::Oyster::Math::Float ls )
	{
		this->lineSpacing = ls;
	}

	template<typename Owner>
	void TextField<Owner>::SetBottomAligned()
	{
		this->isBottomAligned = true;
	}

	template<typename Owner>
	void TextField<Owner>::SetTopAligned()
	{
		this->isBottomAligned = false;
	}

	template<typename Owner>
	unsigned int TextField<Owner>::GetNumLines() const
	{
		return (unsigned int)this->lines.size();
	}

	template<typename Owner>
	void TextField<Owner>::ReserveLines( unsigned int num )
	{
		this->lines.reserve( (::std::vector<::std::wstring>::size_type)num );
	}

	template<typename Owner>
	void TextField<Owner>::ClearText()
	{
		this->lines.resize( 0 );
	}

	template<typename Owner>
	void TextField<Owner>::AppendText( const ::std::wstring &text )
	{
		::std::vector<::std::wstring> split;
		split.reserve( 10 );
		::Utility::String::Split( split, text, L"\n", 0 );
		auto line = split.begin();
		for( ; line != split.end(); ++line )
		{
			this->lines.push_back( (*line) );
		}
	}

	template<typename Owner>
	void TextField<Owner>::PopBack()
	{
		this->lines.pop_back();
	}

	template<typename Owner>
	void TextField<Owner>::PopFront()
	{
		::std::vector<::std::wstring>::size_type i = 0,
												 n = this->lines.size() - 1;
		for( ; i < n; ++i )
		{
			this->lines[i] = this->lines[i+1];
		}

		this->lines.pop_back();
	}
} }

#endif