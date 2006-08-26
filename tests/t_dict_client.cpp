/* 
 * This file part of StarDict - A international dictionary for GNOME.
 * http://stardict.sourceforge.net
 * Copyright (C) 2006 Evgeniy <dushistov@mail.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <cstdlib>
#include <clocale>

#include "dict_client.hpp"

class DictClientTest {
public:
	DictClientTest();
	int run();
private:
	DictClient dict_;
	GMainLoop *main_loop_;

	void on_error(const std::string& mes);
	void on_lookup_end(const DictClient::IndexList&);	
};

DictClientTest::DictClientTest() : dict_("localhost")
{
	main_loop_ = g_main_loop_new(NULL, FALSE);
	dict_.on_error_.connect(sigc::mem_fun(this,
					      &DictClientTest::on_error));
	dict_.on_lookup_end_.connect(sigc::mem_fun(this,
						   &DictClientTest::on_lookup_end));
}

int DictClientTest::run()
{	 
	dict_.lookup_simple("man");	
	g_main_loop_run(main_loop_);
	return EXIT_SUCCESS;
}

void DictClientTest::on_error(const std::string& mes)
{
	g_debug("%s: %s\n", __PRETTY_FUNCTION__, mes.c_str());
}

void DictClientTest::on_lookup_end(const DictClient::IndexList& ilist)
{
	g_debug("%s: %s\n", __PRETTY_FUNCTION__,
		!ilist.empty() ? "found" : "not found");
	for (size_t i = 0; i < ilist.size(); ++i)
		g_debug("--->%s\n%s\n", dict_.get_word(ilist[i]),
			dict_.get_word_data(ilist[i]));
	g_main_loop_quit(main_loop_);
}

int main()
{
	setlocale(LC_ALL, "");//so g_debug and so on will print not garbage
	DictClientTest test;	

	return test.run();
}