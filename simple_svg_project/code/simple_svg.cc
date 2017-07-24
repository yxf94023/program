/**
 *\file simple_svg.cc
 *\date 2017/07/17
 */
#include <stdio.h>
#include <stdlib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xinclude.h>
#include <libxml/xmlstring.h>

int main(void)
{
	xmlDocPtr p_doc;
	xmlNodePtr p_node;
	xmlNodePtr tmp_node;
	int  ret = 0;
	
	p_doc = xmlNewDoc(BAD_CAST"1.0");
	p_node = xmlNewNode(NULL, BAD_CAST"svg");

	xmlNewProp(p_node, BAD_CAST"y", BAD_CAST"0");
	xmlNewProp(p_node, BAD_CAST"x", BAD_CAST"0");
	xmlNewProp(p_node, BAD_CAST"width", BAD_CAST"800");
	xmlNewProp(p_node, BAD_CAST"height", BAD_CAST"800");
	xmlNewProp(p_node, BAD_CAST"viewBox", BAD_CAST"0 0 800 800");
	xmlNewProp(p_node, BAD_CAST"xmlns", BAD_CAST"http://www.w3.org/2000/svg");
	xmlNewProp(p_node, BAD_CAST"xmlns:xlink", BAD_CAST"http://www.w3.org/1999/xlink");
	xmlNewProp(p_node, BAD_CAST"version", BAD_CAST"1.1");
	
	xmlDocSetRootElement(p_doc, p_node);
	
	tmp_node = xmlNewTextChild(p_node, NULL, BAD_CAST"text", BAD_CAST"test hello!");
	xmlNewProp(tmp_node, BAD_CAST"y", BAD_CAST"100");
	xmlNewProp(tmp_node, BAD_CAST"x", BAD_CAST"100");
	
	/* 创建 svg */
	tmp_node = xmlNewNode(NULL, BAD_CAST"svg");
	xmlNewProp(tmp_node, BAD_CAST"x", BAD_CAST"0");
	xmlNewProp(tmp_node, BAD_CAST"y", BAD_CAST"0");
	xmlNewProp(tmp_node, BAD_CAST"width", BAD_CAST"200");
	xmlNewProp(tmp_node, BAD_CAST"height", BAD_CAST"200");
	xmlNewProp(tmp_node, BAD_CAST"viewBox", BAD_CAST"0 0 200 200");
	xmlAddChild(p_node, tmp_node);
	tmp_node = xmlNewTextChild(tmp_node, NULL, BAD_CAST"line", NULL);
	xmlNewProp(tmp_node, BAD_CAST"x1", BAD_CAST"10");
	xmlNewProp(tmp_node, BAD_CAST"y1", BAD_CAST"10");
	xmlNewProp(tmp_node, BAD_CAST"x2", BAD_CAST"50");
	xmlNewProp(tmp_node, BAD_CAST"y2", BAD_CAST"90");
	xmlNewProp(tmp_node, BAD_CAST"style", BAD_CAST"stroke: red; fill: none;");
	
	ret = xmlSaveFile("test.svg", p_doc);
	printf("ret = %d\n", ret);
	
	return 0;
}