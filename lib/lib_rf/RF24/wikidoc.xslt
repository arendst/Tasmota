<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="text" standalone="yes" />
  <xsl:template match="/">
    	<xsl:apply-templates select="//sectiondef[@kind='public-func']/memberdef[@kind='function']" />
  </xsl:template>
  <xsl:template match="memberdef">
  		<!-- xsl:text>&#xA;=== </xsl:text><xsl:value-of select="definition"/> <xsl:value-of select="argsstring"/><xsl:text> ===&#xA;</xsl:text -->
  		<xsl:text>&#xA;=== </xsl:text><xsl:value-of select="name"/><xsl:text> ===&#xA;&#xA;</xsl:text>
  		<xsl:apply-templates select="briefdescription"/>
  		<xsl:if test="count(detaileddescription/para[not(./*)]) &lt; 1"><xsl:text>&#xA;</xsl:text></xsl:if>
  		<xsl:apply-templates select="detaileddescription"/>
  </xsl:template>
  <xsl:template match="briefdescription/para">''<xsl:value-of select="."/>'' </xsl:template>
  <xsl:template match="detaileddescription/para/parameterlist">
Parameters:

<xsl:apply-templates select="parameteritem"/>
  </xsl:template>
  <xsl:template match="parameteritem">* ''<xsl:value-of select="parameternamelist/parametername"/>'': <xsl:value-of select="parameterdescription/para"/><xsl:text>&#xA;</xsl:text>
  </xsl:template>
  <xsl:template match="detaileddescription/para[not(./*)]">
  	<xsl:value-of select="."/><xsl:text>&#xA;</xsl:text>
  </xsl:template>
  <xsl:template match="detaileddescription/para/simplesect[@kind='return']/para">
Returns:

* <xsl:value-of select="."/><xsl:text>&#xA;</xsl:text>
  </xsl:template>
  <xsl:template match="detaileddescription/para/simplesect[@kind='warning']/para">
Warning: <xsl:value-of select="."/><xsl:text>&#xa;</xsl:text>

  </xsl:template>
  <xsl:template match="detaileddescription/para/programlisting">
  	<xsl:text>&#xa;&lt;pre&gt;&#xa;</xsl:text><xsl:apply-templates select="codeline"/><xsl:text>&lt;/pre&gt;&#xa;</xsl:text>
  </xsl:template>
  <xsl:template match="codeline">
  	<xsl:value-of select="."/><xsl:text>&#xa;</xsl:text>
  </xsl:template>
  <xsl:template match="text()"/>
</xsl:stylesheet>

