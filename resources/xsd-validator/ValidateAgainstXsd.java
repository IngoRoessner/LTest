import javax.xml.XMLConstants;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;
import javax.xml.validation.Validator;
import java.io.File;

public class ValidateAgainstXsd {

    public static void main(String[] args){
        if(args.length == 2){
            	String xml_file = args[0];
            	String xsd_file = args[1];
            	boolean result = validateAgainstXsd(new File(xml_file), new File(xsd_file));
            	System.exit(result == true ? 0 : 1);
        } else {
        	System.out.println("XML on XML schema validator\nUSAGE: xml_filepath xsd_filepath");
        }
    }

    private static boolean validateAgainstXsd(File xml, File xsd){
	    try {
    		SchemaFactory factory = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
    		Schema schema = factory.newSchema(xsd);
    		Validator validator = schema.newValidator();
    		validator.validate(new StreamSource(xml));
    		return true;
    	} catch(Exception ex){
            ex.printStackTrace();
    		return false;
    	}
    }
}
